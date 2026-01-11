#include "MusicPlayer.h"
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>      
#include <windows.h>

/* =============================================================
 * GLOBAL STATE & SYNCHRONIZATION
 * ============================================================= */

static std::mutex audioMutex;
static std::condition_variable audioCV;

/* Thread control flags */
static std::atomic<bool> audioRunning = true;       /* Is the audio thread alive? */
static std::atomic<bool> requestNewSong = false;    /* User requested a track change */
static std::atomic<bool> requestPause = false;      /* User requested pause */

/* Playback state */
static std::atomic<bool> isPaused = false;
static Song audioSong;                 /* The song object currently held by the thread */

/* Constants for timing */
static constexpr int LOOP_DELAY_MS = 100;
static constexpr int RESUME_DELAY_MS = 50;

/* Forward declaration */
static void audioThreadFunc(MusicPlayer* player);

/* =============================================================
 * CLASS IMPLEMENTATION
 * ============================================================= */

MusicPlayer::MusicPlayer()
{
    /* Load music library from CSV at initialization. */
    loadLibraryFromCSV("data/playlist.csv", library);

    /* Initialize all indexes for fast lookup. */
    library.initializeSongByID();
    library.initializeSongByTitle();
    library.initializeSongByArtist();
    library.initializeSongByAlbum();

    /* Start the background audio processing thread. */
    static std::thread audioThread(audioThreadFunc, this);
    audioThread.detach();
}

void MusicPlayer::selectAndPlaySong(int songID)
{
    /* Find the requested song in the library. */
    Song* song = library.findSongByID(songID);

    /* Return immediately if not found */
    if (song == nullptr)
    {
        std::cerr << "[Error] Song ID " << songID << " not found in library.\n";
        return; 
    }

    /* If a song is currently playing, push it to playback history. */
    if (hasCurrentSong)
    {
        playbackHistory.pushSong(currentSong);
    }

    /* Update current song state. */
    currentSong = *song;
    hasCurrentSong = true;
    isPaused = false;

    /* Add selected song to the playback queue. */
    playbackQueue.addSong(currentSong);

    /* Trigger playback. */
    playSong(currentSong);
}

void MusicPlayer::addSongToPlayNext(int id)
{
    Song* song = library.findSongByID(id);

    /* ERROR HANDLING */
    if (song == nullptr)
    {
        std::cerr << "[Error] Cannot add to queue: Song ID " << id << " not found.\n";
        return; 
    }

    /* Add song to the high-priority queue. */
    playNextQueue.addSong(*song);
    std::cout << "Added '" << song->title << "' to Play Next queue.\n";
}

void MusicPlayer::enableShuffle()
{
    ShuffleManager shuffleManager;
    std::vector<Song*> playlist;

    /* Add all songs to the playlist before shuffle */
    for (Song& song : library.getSongs())
    {
        playlist.push_back(&song);
    }

    if (playlist.empty())
    {
        std::cerr << "[Warning] Library is empty, cannot shuffle.\n";
        return;
    }

    /* Randomize the order. */
    shuffleManager.initialize(playlist);

    /* Create a new queue for the shuffled order. */
    PlaybackQueue shuffledQueue;

    while (true)
    {
        Song* nextSong = shuffleManager.getNextSong();
        if (nextSong == nullptr)
        {
            break;
        }
        shuffledQueue.addSong(*nextSong);
    }

    /* Replace the current queue. */
    playbackQueue = shuffledQueue;

    std::cout << "Shuffle enabled: playback queue updated.\n";
}

void MusicPlayer::BFS(int startSongID, int maxSize)
{
    Song* startSong = library.findSongByID(startSongID);

    /* ERROR HANDLING */
    if (startSong == nullptr)
    {
        std::cerr << "[Error] BFS failed: Start song ID " << startSongID << " not found.\n";
        return;
    }

    /* Generate a smart playlist based on graph traversal. */
    PlaybackQueue smartQueue = generateSmartPlaylist(*startSong, library, maxSize);

    playbackQueue = smartQueue;

    std::cout << "Smart playlist generated using BFS.\n";
}

void MusicPlayer::playNext()
{
    /* Archive current song to history. */
    if (hasCurrentSong)
    {
        playbackHistory.pushSong(currentSong);
    }

    /* Priority 1: Check the "Play Next" specific queue. */
    if (!playNextQueue.isEmpty())
    {
        std::cout << "Playing from PlayNextQueue...\n";
        currentSong = playNextQueue.playNext();
    }
    /* Priority 2: Continue with the standard playback queue. */
    else if (!playbackQueue.isEmpty())
    {
        std::cout << "Playing from PlaybackQueue...\n";      
        currentSong = playbackQueue.getCurrentSong();
        playbackQueue.playNext();
    }
    else
    {
        /* ERROR HANDLING: Just print and stop, don't crash */
        std::cerr << "[Info] End of playlist. No more songs to play.\n";
        return;
    }

    hasCurrentSong = true;
    isPaused = false;

    /* Trigger playback. */
    playSong(currentSong);
}

void MusicPlayer::playPrevious()
{
    /* ERROR HANDLING */
    if (playbackHistory.isEmpty())
    {
        std::cerr << "[Warning] No previous song in history.\n";
        return;
    }

    /* Retrieve last song (LIFO). */
    currentSong = playbackHistory.playPreviousSong();
    hasCurrentSong = true;

    playSong(currentSong);
}

/* --- Getters & Setters --- */

MusicLibrary& MusicPlayer::getLibrary()
{
    return library;
}

PlaybackQueue& MusicPlayer::getPlaybackQueue()
{
    return playbackQueue;
}

PlayNextQueue& MusicPlayer::getPlayNextQueue()
{
    return playNextQueue;
}

PlaybackHistory& MusicPlayer::getPlaybackHistory()
{
    return playbackHistory;
}

void MusicPlayer::setPlaybackQueue(PlaybackQueue& pb)
{
    playbackQueue = pb;
}

/* =============================================================
 * GLOBAL FUNCTIONS (Main Thread Interface)
 * ============================================================= */

void playSong(const Song& song)
{
    std::cout << "\n-----------------------------------\n";
    std::cout << " Now playing:\n";
    std::cout << "   ID      : " << song.id << "\n";
    std::cout << "   Title   : " << song.title << "\n";
    std::cout << "   Artist  : " << song.artist << "\n";
    std::cout << "   Duration: " << song.duration << " s\n";
    std::cout << "-----------------------------------\n";

    {
        std::lock_guard<std::mutex> lock(audioMutex);
        audioSong = song;
        requestNewSong = true;
        /* Reset flags when starting a new song */
        requestPause = false; 
    }

    /* Notify the audio thread to wake up. */
    audioCV.notify_one();
}

void pauseSong()
{
    /* Flag the request; the thread will handle the MCI command. */
    requestPause = true;
    std::cout << "[Action] Pause requested.\n";
}

void resumeSong()
{
    /* Flag the request only if currently paused. */
    requestPause = false;
    std::cout << "[Action] Resume requested.\n";
}

void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        /* ERROR HANDLING*/
        std::cerr << "[Critical Error] Failed to open CSV file: " << filePath << "\n";
        std::cerr << "Please check if 'data/playlist.csv' exists.\n";
        return;
    }

    std::string line;
    /* Skip the CSV header. */
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty()) continue; // Skip empty lines

        std::stringstream ss(line);
        std::string token;
        Song song;

        try {
            /* Parse CSV columns */
            std::getline(ss, token, ',');
            song.id = std::stoi(token);

            std::getline(ss, song.title, ',');
            std::getline(ss, song.artist, ',');
            std::getline(ss, song.album, ',');

            std::getline(ss, token, ',');
            song.duration = std::stoi(token);

            std::getline(ss, song.path); /* Last column */

            library.addSong(song);
        }
        catch (...)
        {
            std::cerr << "[Warning] Skipping malformed line in CSV.\n";
            continue;
        }
    }
}

/* =============================================================
 * BACKGROUND AUDIO THREAD
 * ============================================================= */

static void audioThreadFunc(MusicPlayer* player)
{
    while (audioRunning)
    {
        Song songToPlay;

        /* Wait for a new song request */
        {
            std::unique_lock<std::mutex> lock(audioMutex);
            audioCV.wait(lock, []() { return requestNewSong || !audioRunning; });

            if (!audioRunning) break;

            songToPlay = audioSong;
            requestNewSong = false;
        }
        
        /* Stop any previous track */
        mciSendString("stop music", NULL, 0, NULL);
        mciSendString("close music", NULL, 0, NULL);

        /* Open the new track */
        std::string openCmd = "open \"" + songToPlay.path + "\" type waveaudio alias music";
        mciSendString(openCmd.c_str(), NULL, 0, NULL);
        
        /* Start playing */
        mciSendString("play music", NULL, 0, NULL);

        /* Reset state flags for the new track */
        isPaused = false;
        requestPause = false;

        /* Monitor Playback Loop */
        while (true)
        {
            //* Handle Pause / Resume */
            if (requestPause && !isPaused)
            {
                mciSendString("pause music", NULL, 0, NULL);
                isPaused = true;
            }
            else if (!requestPause && isPaused)
            {
                mciSendString("resume music", NULL, 0, NULL);
                isPaused = false;

                std::this_thread::sleep_for(std::chrono::milliseconds(RESUME_DELAY_MS));
            }

            /* Check Playback Status */
            char status[128] = {0};
            mciSendString("status music mode", status, sizeof(status), NULL);

            /* Check if user requested to skip/change song */
            if (requestNewSong) 
            {
                break;
            }

            /* Check if song finished naturally. */
            if (std::string(status) == "stopped" && !isPaused)
            {
                player->playNext(); 
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY_MS));
        }
    }

    /* Cleanup before thread exit */
    mciSendString("close music", NULL, 0, NULL);
}