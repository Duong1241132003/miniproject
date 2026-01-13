#ifndef PLAYBACK_HISTORY_H
#define PLAYBACK_HISTORY_H

#include <stack>
#include "Song.h"

/*
 * PlaybackHistory
 * ----------------
 * Stores playback history using a LIFO stack.
 * Supports "Back" button functionality.
 */
class PlaybackHistory
{
private:
    /*
     * Stack storing played songs.
     * Top of stack = most recently played song.
     */
    std::stack<Song> history;
    
public:
    /*
     * Adds a song to playback history.
     * Should be called when a song finishes playing.
     */
    void pushSong(const Song& song);

    /*
     * Returns the previously played song.
     * Removes the song from history.
     */
    Song playPreviousSong();

    /*
     * Checks whether playback history is empty.
     */
    bool isEmpty() const;

    /*
     * Print all songs in the history
     */
    void printHistory() const;

};

#endif
