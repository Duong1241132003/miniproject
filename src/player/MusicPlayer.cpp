#include "MusicPlayer.h"
#include <stdexcept>

MusicPlayer::MusicPlayer()
{
    /*
     * Load music library from CSV at initialization.
     */
    loadLibraryFromCSV("data/playlist.csv", library);
    
    // /*
    //  * Initialize playqueue
    //  */
    // for(Song& i:library.getSongs())
    // {
    //     playbackQueue.addSong(i);
    // }

    /*
     * Initialize all indexes for fast lookup.
     */

    library.initializeSongByID();
    library.initializeSongByTitle();
    library.initializeSongByArtist();
    library.initializeSongByAlbum();
}

void MusicPlayer::selectAndPlaySong(int songID)
{
    /*
     * Find the requested song in the library.
     */
    Song* song = library.findSongByID(songID);

    if (song == nullptr)
    {
        throw std::runtime_error("Song not found");
    }

    /*
     * If a song is currently playing,
     * push it to playback history.
     */
    if (hasCurrentSong)
    {
        playbackHistory.pushSong(currentSong);
    }

    /*
     * Set the selected song as current.
     */
    currentSong = *song;
    hasCurrentSong = true;

    /*
     * play selected song
     */
    playSong(currentSong);
}

void MusicPlayer::addSongToPlayNext(int id)
{
    /*
     * Find song by ID.
     */
    Song* song = library.findSongByID(id);

    if (song == nullptr)
    {
        throw std::runtime_error("Song not found");
    }

    /*
     * Add song to playNextQueue
     */
    playNextQueue.addSong(*song);
}

void MusicPlayer::enableShuffle()
{
    /*
     * Create a local ShuffleManager instance.
     * No persistent shuffle state is required.
     */
    ShuffleManager shuffleManager;

    /*
     * Prepare a list of pointers to all songs in the library.
     * ShuffleManager operates on Song* instead of Song objects.
     */
    std::vector<Song*> playlist;

    for (Song& song : library.getSongs())
    {
        playlist.push_back(&song);
    }

    /*
     * Initialize shuffle order using ShuffleManager.
     * This randomizes the song order internally.
     */
    shuffleManager.initialize(playlist);

    /*
     * Create a new playback queue that will store
     * the shuffled playback order.
     */
    PlaybackQueue shuffledQueue;

    /*
     * Retrieve shuffled songs one by one
     * and add them to the playback queue.
     */
    while (true)
    {
        Song* nextSong = shuffleManager.getNextSong();

        /*
         * When nullptr is returned,
         * all songs have been consumed.
         */
        if (nextSong == nullptr)
        {
            break;
        }

        shuffledQueue.addSong(*nextSong);
    }

    /*
     * Replace the current playback queue
     * with the shuffled playback queue.
     */
    playbackQueue = shuffledQueue;

    /*
     * Notify user that shuffle mode has been applied.
     */
    std::cout << "Shuffle enabled: playback queue updated\n";
}


void MusicPlayer::BFS(int startSongID, int maxSize)
{
    /*
     * Find the starting song by its ID.
     */
    Song* startSong = library.findSongByID(startSongID);

    if (startSong == nullptr)
    {
        throw std::runtime_error("Start song not found");
    }

    /*
     * Generate a smart playlist using BFS traversal.
     */
    PlaybackQueue smartQueue =
        generateSmartPlaylist(*startSong, library, maxSize);

    /*
     * Replace the current playback queue with the generated one.
     */
    playbackQueue = smartQueue;

    /*
     * Notify user that smart playlist has been generated.
     */
    std::cout << "Smart playlist generated using BFS\n";
}


void MusicPlayer::playNext()
{
    /*
     * Save the current song to playback history
     * before switching to the next track.
     */
    if (hasCurrentSong)
    {
        playbackHistory.pushSong(currentSong);
    }

    /*
     * Priority 1:
     * Play songs explicitly marked as "Play Next".
     */
    if (!playNextQueue.isEmpty())
    {
        std::cout << "Playing from PlayNextQueue\n";
        currentSong = playNextQueue.playNext();
    }
    /*
     * Priority 2:
     * Continue normal playback order.
     */
    else if(!playbackQueue.isEmpty())
    {
        std::cout << "Playing from PlaybackQueue\n";
        playbackQueue.playNext();
        currentSong = playbackQueue.getCurrentSong();
    }
    else
    {
        throw std::runtime_error("No songs available to play next");
    }

    /*
     * Update playback state.
     */
    playSong(currentSong);
    hasCurrentSong = true;
}

void MusicPlayer::playPrevious()
{
    /*
     * Check whether playback history is empty.
     * If empty, there is no previous song to return to.
     */
    if (playbackHistory.isEmpty())
    {
        throw std::runtime_error("No previous song in history");
    }

    /*
     * Retrieve the most recently played song from history.
     * This operation removes the song from the stack (LIFO).
     */
    currentSong = playbackHistory.playPreviousSong();

    /*
     * Update playback state to indicate a song is active.
     */
    hasCurrentSong = true;

    /*
     * Start playing the retrieved song.
     */
    playSong(currentSong);
}


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

void playSong(const Song& song)
{
    std::cout << "Now playing:\n";
    std::cout << "  ID      : " << song.id << "\n";
    std::cout << "  Title   : " << song.title << "\n";
    std::cout << "  Artist  : " << song.artist << "\n";
    std::cout << "  Album   : " << song.album << "\n";
    std::cout << "  Duration: " << song.duration << " s\n";
    std::cout << "  Path    : " << song.path << "\n";
    std::cout << "-----------------------------------\n";
    PlaySoundA(
                    song.path.c_str(),
                    NULL,
                    SND_FILENAME | SND_SYNC
                );
}

void pauseSong()
{
    /*
     * Stop current playback immediately.
     * This is a simulation using Windows PlaySound API.
     */
    PlaySound(NULL, 0, 0);

    /*
     * Inform the user that playback is paused.
     */
    std::cout << "Playback paused\n";
}


/*
 * Loads songs from a CSV file into the music library.
 * Expected CSV format
 * id,title,artist,album,duration,path
 */
void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file");
    }

    std::string line;

    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        Song song;

        // id
        std::getline(ss, token, ',');
        std::cout << "Parsing id = [" << token << "]\n";
        song.id = std::stoi(token);

        // title
        std::getline(ss, song.title, ',');

        // artist
        std::getline(ss, song.artist, ',');

        // album
        std::getline(ss, song.album, ',');

        // duration
        std::getline(ss, token, ',');
        song.duration = std::stoi(token);

        // path (last column)
        std::getline(ss, song.path);

        library.addSong(song);
    }
}

