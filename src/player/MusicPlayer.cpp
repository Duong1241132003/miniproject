#include "MusicPlayer.h"
#include <stdexcept>

MusicPlayer::MusicPlayer()
{
    /*
     * Load music library from CSV at initialization.
     */
    loadLibraryFromCSV("data/playlist.csv", library);

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
     * Add the selected song.
     */
    playbackQueue.addSong(currentSong);
}

void MusicPlayer::addSongToPlayNext(const Song& song)
{
    /*
     * Add the song to the PlayNextQueue.
     */
    playNextQueue.addSong(song);
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

MusicLibrary& MusicPlayer::getLibrary()
{
    return library;
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