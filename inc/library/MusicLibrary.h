#ifndef MUSIC_LIBRARY_H
#define MUSIC_LIBRARY_H

#include <vector>
#include <map>
#include <unordered_map>
#include "Song.h"

/*
 * MusicLibrary owns and manages the complete collection of songs.
 * The library is expected to be loaded once at startup and rarely modified.
 */
class MusicLibrary
{
private:
    /*
     * Main storage: owns all Song objects.
     */
    std::vector<Song> songs;

    /*
     * Key   : song ID
     * Value : song stored in vector
     */
    std::unordered_map<int, Song*> songByID;

    /*
     * Key   : song title
     * Value : song stored in vector
     */
    std::map<std::string, Song*> songByTitle;

    /*
     * Key   : artist name
     * Value : list of songs by that artist
     */
    std::unordered_map<std::string, std::vector<Song*>> songByArtist;

    /*
     * Key   : album name
     * Value : list of songs in that album
     */
    std::unordered_map<std::string, std::vector<Song*>> songByAlbum;

public:
    /*
     * Loads the music library from a CSV file.
     */
    void loadLibraryFromCSV(const std::string& filePath);

    /*
     * Adds a song to the library.
     */
    void addSong(const Song& song);

    /*
     * Provides fast random access by index.
     */
    const Song& getSongByIndex(size_t index) const;

    /*
     * Finds a song by its unique ID.
     * Returns nullptr if the song does not exist.
     */
    Song* findSongByID(int id);

     /*
     * Finds a song by its title.
     * Returns nullptr if not found.
     */
    Song* findSongByTitle(const std::string& title);

    /*
    * Finds all songs by a given artist.
    * Returns empty vector if artist not found.
    */
    std::vector<Song*> findSongsByArtist(const std::string& artist);

    /*
    * Finds all songs in a given album.
    * Returns empty vector if album not found.
    */
    std::vector<Song*> findSongsByAlbum(const std::string& album);
    /*
     * Returns total number of songs.
     */
    size_t getSongCount() const;

    /* 
     * Initializes the songByID. 
     * Must be called after loading all songs.
     */
    void initializeSongByID();

    /* 
     * Initializes the songByTitle . 
     * Must be called after loading all songs.
     */
    void initializeSongByTitle();

     /*
     * Initializes the songByArtist .
     * Must be called after loading all songs.
     */
    void initializeSongByArtist();

    /*
     * Initializes the songByAlbum .
     * Must be called after loading all songs.
     */
    void initializeSongByAlbum();

    /*
     * Get all song
     */
    std::vector<Song>& getSongs();

};


#endif
