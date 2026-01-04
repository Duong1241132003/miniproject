#include "PlaybackHistory.h"
#include <stdexcept>

void PlaybackHistory::pushSong(const Song& song)
{
    history.push(song);
}

Song PlaybackHistory::playPreviousSong()
{
    if (history.empty())
    {
        throw std::runtime_error("Playback history is empty");
    }

    Song previousSong = history.top();
    history.pop();

    return previousSong;
}

bool PlaybackHistory::isEmpty() const
{
    return history.empty();
}
