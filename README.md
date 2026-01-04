CPP.MiniProject/
│
├── CMakeLists.txt              # (Tuỳ chọn) build bằng CMake
├── README.md                   # Mô tả project, hướng dẫn build
│
├── inc/                    # Header files (.h)
│   │
│   ├── model/                  # Các cấu trúc dữ liệu cơ bản
│   │   ├── Song.h
│   │
│   ├── library/                # Quản lý thư viện nhạc
│   │   ├── MusicLibrary.h
│   │
│   ├── playback/               # Phát nhạc & queue
│   │   ├── PlaybackQueue.h
│   │   ├── PlaybackHistory.h
│   │   ├── ShuffleManager.h
│   │
│   ├── player/                 # Lớp tích hợp hệ thống
│   │   ├── MusicPlayer.h
│   │
│   └── algorithm/              # Thuật toán nâng cao
│       └── SmartPlaylist.h
│
├── src/                        # Source files (.cpp)
│   │
│   ├── model/
│   │   └── Song.cpp
│   │
│   ├── library/
│   │   └── MusicLibrary.cpp
│   │
│   ├── musicplayer/
│   │   ├── PlaybackQueue.cpp
│   │   ├── PlayNextQueue.cpp
│   │   ├── PlaybackHistory.cpp
│   │   └── ShuffleManager.cpp
│   │
│   ├── player/
│   │   └── MusicPlayer.cpp
│   │
│   ├── algorithm/
│   │   └── SmartPlaylist.cpp
│   │
│   └── main.cpp                # Hàm main – demo & test
│
├── data/                       # Dữ liệu mẫu
│   └── songs.csv               # Danh sách bài hát test
│
├── docs/                       # Báo cáo & tài liệu
│   ├── DesignReport.md         # Phân tích cấu trúc dữ liệu
│   └── ComplexityAnalysis.md  # Phân tích Big-O
│
└── tests/                      # (Tuỳ chọn) Unit test
    ├── test_library.cpp
    ├── test_queue.cpp
    └── test_shuffle.cpp
