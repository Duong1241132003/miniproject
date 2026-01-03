#ifndef MUSIC_LIBRARY_H
#define MUSIC_LIBRARY_H

#include <vector>
#include "Song.h"

/*
 * MusicLibrary owns and manages the complete collection of songs.
 * The library is expected to be loaded once at startup and rarely modified.
 */
class MusicLibrary
{
public:
    /*
     * Adds a song to the library.
     * Chosen as push_back because insertions are infrequent and append-only.
     */
    void addSong(const Song& song);

    /*
     * Provides fast random access to songs by index.
     * Throws an exception if the index is invalid.
     */
    const Song& getSongByIndex(size_t index) const;

    /*
     * Returns the total number of songs in the library.
     */
    size_t getSongCount() const;

private:
    /*
     * std::vector is used for:
     *  - O(1) random access
     *  - contiguous memory (cache-friendly)
     *  - minimal overhead for large datasets
     */
    std::vector<Song> songs;
};

#endif
