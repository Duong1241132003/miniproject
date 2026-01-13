#include "MusicPlayer.h"
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono> 
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
static std::atomic<bool> repeatEnabled = false;     /* Controls continuous replay of current song */

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
    library.loadLibraryFromCSV("data/playlist.csv");

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

void MusicPlayer::printPlayNextQueue() const
{
    std::cout << "\n--- PLAY NEXT QUEUE ---\n";

    if (playNextQueue.isEmpty())
    {
        std::cout << "(empty)\n";
        return;
    }

    playNextQueue.printAllSongs();
}


void MusicPlayer::enableShuffle()
{
    /* Prevent enabling Shuffle twice */
    if (shuffleEnabled)
    {
        std::cerr << "[Info] Shuffle already enabled.\n";
        return;
    }

    /* Save original queue if no mode is active */
    if (!baseQueueSaved)
    {
        baseQueue = playbackQueue;
        baseQueueSaved = true;
    }

    shuffleEnabled = true;

    /* Shuffle SmartPlaylist if active, otherwise base queue */
    if (smartPlaylistEnabled)
    {
        playbackQueue = applyShuffle(smartQueue);
    }
    else
    {
        playbackQueue = applyShuffle(baseQueue);
    }

    std::cout << "Shuffle enabled.\n";
}

void MusicPlayer::disableShuffle()
{
    /* Prevent disabling if Shuffle is not active */
    if (!shuffleEnabled)
    {
        std::cerr << "[Info] Shuffle not enabled.\n";
        return;
    }

    /* Disable Shuffle */
    shuffleEnabled = false;

    /* Restore SmartPlaylist if still active */
    if (smartPlaylistEnabled)
    {
        playbackQueue = smartQueue;
    }
    else
    {
        playbackQueue = baseQueue;
        baseQueueSaved = false;
    }

    std::cout << "Shuffle disabled.\n";
}


void MusicPlayer::enableSmartPlaylist(int startSongID, int maxSize)
{
    /* Prevent enabling SmartPlaylist twice */
    if (smartPlaylistEnabled)
    {
        std::cerr << "[Info] Smart playlist already enabled.\n";
        return;
    }

    /* Find starting song */
    Song* startSong = library.findSongByID(startSongID);

    /* Abort if song not found */
    if (startSong == nullptr)
    {
        std::cerr << "[Error] Start song not found.\n";
        return;
    }

    /* Save original queue only once */
    if (!baseQueueSaved)
    {
        baseQueue = playbackQueue;
        baseQueueSaved = true;
    }

    /* Generate SmartPlaylist queue */
    smartQueue = generateSmartPlaylist(*startSong, library, maxSize);
    smartPlaylistEnabled = true;

    /* Apply shuffle on top of SmartPlaylist if active */
    if (shuffleEnabled)
    {
        playbackQueue = applyShuffle(smartQueue);
    }
    else
    {
        playbackQueue = smartQueue;
    }

    std::cout << "Smart playlist enabled.\n";
}

void MusicPlayer::disableSmartPlaylist()
{
    /* Prevent disabling if SmartPlaylist is not active */
    if (!smartPlaylistEnabled)
    {
        std::cerr << "[Info] Smart playlist not enabled.\n";
        return;
    }

    /* Disable SmartPlaylist */
    smartPlaylistEnabled = false;

    /* Restore Shuffle if still enabled */
    if (shuffleEnabled)
    {
        playbackQueue = applyShuffle(baseQueue);
    }
    else
    {
        playbackQueue = baseQueue;
        baseQueueSaved = false;
    }

    std::cout << "Smart playlist disabled.\n";
}

PlaybackQueue MusicPlayer::applyShuffle(PlaybackQueue& source)
{
    /* Initialize ShuffleManager and prepare playlist */
    ShuffleManager shuffleManager;
    std::vector<Song*> playlist;

    /* Push all songs from the source queue into the playlist vector */
    for (Song& s : source.getQueue())
    {
        playlist.push_back(&s);
    }

    /* Initialize and generate shuffled queue */
    shuffleManager.initialize(playlist);

    /* Create a new PlaybackQueue with the shuffled songs */
    PlaybackQueue result;

    /* Add all shuffled songs to the result queue */
    while (Song* s = shuffleManager.getNextSong())
    {
        result.addSong(*s);
    }

    return result;
}

void MusicPlayer::enableRepeat()
{
    /* Enable continuous replay of the current song */
    repeatEnabled = true;
    std::cout << "Repeat enabled.\n";
}

void MusicPlayer::disableRepeat()
{
    /* Disable continuous replay and allow normal playlist progression */
    repeatEnabled = false;
    std::cout << "Repeat disabled.\n";
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
                    /* Restart the same song if repeat mode is enabled */
                if (repeatEnabled)
                {
                    playSong(songToPlay);
                    break;
                }

                /* Advance normally when repeat mode is disabled */
                player->playNext(); 
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY_MS));
        }
    }

    /* Cleanup before thread exit */
    mciSendString("close music", NULL, 0, NULL);
}