    #include <SFML/Audio.hpp>
    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <cmath>
    #include <filesystem>
    const float PI = 3.14159265351;

    class MusicAndVideoPlayer {
    public:
        MusicAndVideoPlayer() : window(sf::VideoMode(1080, 720), "SFML Music and Video Player"), selectedFileIndex(-1) {
            if (!font.loadFromFile("broken_geek.otf")) {
                std::cerr << "Error loading font." << std::endl;
            }
            text.setFillColor(sf::Color::Red);
            text.setFont(font);
            text.setCharacterSize(20);
            text.setPosition(10.f, 10.f);

            // Initialize imageTexture
            imageTexture.loadFromFile("");  // Provide a default or empty path

            updateFileList();
        }

        void run() {
            while (window.isOpen()) {
                handleEvents();
                render();
            }
        }

    private:
        sf::RenderWindow window;
        sf::Font font;
        sf::Text text;
        std::vector<std::string> fileList;
        int selectedFileIndex;
        sf::Music music;
        sf::Sprite image;
        sf::Texture imageTexture;  // Move imageTexture here
        sf::Text songNameText;

        // Add this function to generate the path for the image
        std::string getImagePathForFile(const std::string& fileName) {
            // Assuming images have the same name as the song with a different extension
            return fileName.substr(0, fileName.find_last_of('.')) + ".jpeg";
        }

        void updateFileList() {
            // Simulated list of files
            fileList = { "1.mp3", "2.mp3", "3.mp3", "4.mp3" };

            // Update the display text
            std::string displayText = "Select a file to play:\n";
            for (size_t i = 0; i < fileList.size(); ++i) {
                displayText += std::to_string(i + 1) + ". " + fileList[i] + "\n";
            }
            text.setString(displayText);
        }


        void handleEvents() {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                    }
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        togglePlayPause();
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        playNextSong();
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        playPreviousSong();
                    }
                    else if (event.key.code == sf::Keyboard::Up) {
                        increaseVolume();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        decreaseVolume();
                    }
                }
            }
        }

        void handleMouseClick(float mouseX, float mouseY) {
            // Check if the click is within the bounds of a file entry
            for (size_t i = 0; i < fileList.size(); ++i) {
                float entryTop = 40.f + i * 30.f;
                float entryBottom = entryTop + 25.f;

                if (mouseY > entryTop && mouseY < entryBottom) {
                    selectedFileIndex = static_cast<int>(i);
                    playSelectedFile();
                    return;
                }
            }
        }

        void playSelectedFile() {
            if (selectedFileIndex >= 0 && selectedFileIndex < static_cast<int>(fileList.size())) {
                std::string selectedFile = fileList[selectedFileIndex];
                std::cout << "Selected file: " << selectedFile << std::endl;

                // Stop the current music
                music.stop();

                // Load and play the selected file using sf::Music
                if (music.openFromFile(selectedFile)) {
                    music.play();

                    // Set the image based on the selected file
                    std::string imagePath = getImagePathForFile(selectedFile);
                    if (imageTexture.loadFromFile(imagePath)) {
                        image.setTexture(imageTexture);
                        // Adjust size and position as needed
                    // reduceImageSize(0.6f);
                        image.setPosition(20.f, 20.f);
                    }
                    else {
                        std::cerr << "Error loading image for the selected file." << std::endl;
                    }

                    playMusicWithInterference();
                }
                else {
                    std::cerr << "Error loading and playing the selected file." << std::endl;
                }
            }
        }

        void togglePlayPause() {
            if (music.getStatus() == sf::Music::Playing) {
                music.pause();
            }
            else {
                music.play();
            }
        }

        void playNextSong() {
            selectedFileIndex = (selectedFileIndex + 1) % fileList.size();
            playSelectedFile();
        }

        void playPreviousSong() {
            selectedFileIndex = (selectedFileIndex - 1 + fileList.size()) % fileList.size();
            playSelectedFile();
        }

        void increaseVolume() {
            float newVolume = music.getVolume() + 10.0f;
            music.setVolume(std::min(newVolume, 100.0f));  // Limit volume to 30
        }

        void decreaseVolume() {
            float newVolume = music.getVolume() - 10.0f;
            music.setVolume(std::max(newVolume, 0.0f));    // Limit volume to 0
        }
        void render() {
            window.clear(sf::Color::White);
            window.draw(text);
            window.draw(image);  // Draw the image

            window.display();
        }



        void playMusicWithInterference() {
            // Additional variables for control
            bool isPlaying = true; // Initially playing
            bool isPaused = false;

            // Load background image
            sf::Texture backgroundTexture;
            if (!backgroundTexture.loadFromFile("intro.jpeg")) {
                std::cerr << "Error loading background image." << std::endl;
                return;
            }
            sf::Sprite background(backgroundTexture);

            // Load play, pause, next, and previous button textures
            sf::Texture playTexture;
            sf::Texture nextTexture;
            sf::Texture prevTexture;
            sf::Texture pauseTexture;
            if (!playTexture.loadFromFile("play.png") ||
                !nextTexture.loadFromFile("next.png") ||
                !prevTexture.loadFromFile("pervi.png") ||
                !pauseTexture.loadFromFile("pause.png")) {
                std::cerr << "Error loading button textures." << std::endl;
                return;
            }

            // Create sprites for play/pause, next, and previous buttons
            sf::Sprite playButton(playTexture);
            sf::Sprite nextButton(nextTexture);
            sf::Sprite prevButton(prevTexture);
            sf::Sprite pauseButton(pauseTexture);

            playButton.setPosition(window.getSize().x / 2 - 35, 500);  // Adjust position as needed
            nextButton.setPosition(window.getSize().x / 2 + 81, 514); // Adjust position as needed
            prevButton.setPosition(window.getSize().x / 2 - 150, 514); // Adjust position as needed
            pauseButton.setPosition(window.getSize().x / 2 - 35, 500);

            // Create a vertex array for the wave
            const int numPoints = 800;
            sf::VertexArray wave(sf::LineStrip, numPoints);
            for (int i = 0; i < numPoints; ++i) {
                wave[i].position = sf::Vector2f(i * window.getSize().x / numPoints, window.getSize().y - 130);
                wave[i].color = sf::Color::Magenta;
            }
            // Volume bar
            sf::Color Color(98, 37, 124);

            sf::RectangleShape volumeBar;
            float volume = music.getVolume() * 20.0f;  // Normalize volume to a range of 0 to 1
            volumeBar.setSize(sf::Vector2f(600.f, 1000.f * volume));  // Set size to 30x30 pixels
            // volumeBar.setSize(sf::Vector2f(30.f, 30.f));
            volumeBar.setFillColor(sf::Color::Yellow);
            volumeBar.setPosition(window.getSize().x - 150, window.getSize().y - 40.f);

            //Seek bar
            sf::RectangleShape seekBar;
            seekBar.setSize(sf::Vector2f(window.getSize().x, 10.f));
            seekBar.setFillColor(Color);
            seekBar.setPosition(0.f, window.getSize().y - 10.f);

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Space) {
                            if (isPlaying) {
                                music.pause();
                                isPaused = true;
                            }
                            else {
                                music.play();
                                isPaused = false;
                            }
                            isPlaying = !isPlaying;
                        }
                        else if (event.key.code == sf::Keyboard::N) {
                            // Play next song in the playlist
                            playNextSong();
                        }
                        else if (event.key.code == sf::Keyboard::P) {
                            // Play previous song in the playlist
                            playPreviousSong();
                        }
                        else if (event.key.code == sf::Keyboard::Up) {
                            // Increase volume
                            increaseVolume();
                        }
                        else if (event.key.code == sf::Keyboard::Down) {
                            // Decrease volume
                            decreaseVolume();
                        }
                    }
                }

                // Update the volume bar based on the current volume
                float volume = music.getVolume() / 100.0f;  // Normalize volume to a range of 0 to 1
                volumeBar.setSize(sf::Vector2f(150.f * volume, 30.f));  // Set size to 30x30 pixels

                // Update the wave based on the music
                if (isPlaying && !isPaused) {
                    sf::Time elapsedTime = music.getPlayingOffset();
                    float waveFrequency = 50.1;  // Adjust as needed
                    float waveAmplitude = 50.0;  // Adjust as needed

                    for (int i = 0; i < numPoints; ++i) {
                        float x = i * window.getSize().x / numPoints;
                        float y = window.getSize().y / 2 + waveAmplitude * std::sin(2 * PI * waveFrequency * x + elapsedTime.asSeconds());
                        wave[i].position = sf::Vector2f(x, y);
                    }
                }

                // Update the seek bar based on the music playback progress
                float musicDuration = music.getDuration().asSeconds();
                float progress = music.getPlayingOffset().asSeconds() / musicDuration;
                seekBar.setSize(sf::Vector2f(window.getSize().x * progress, 10.f));

                window.clear();
                // Draw background
                window.draw(background);
                window.draw(image);
                // Draw play or pause button based on the current state
                if (isPlaying) {
                    window.draw(pauseButton);
                }
                else {
                    window.draw(playButton);
                }
                // Draw next and previous buttons
                window.draw(nextButton);
                window.draw(prevButton);
                // Draw the wave, volume bar, and seek bar
                window.draw(wave);
                window.draw(volumeBar);
                window.draw(seekBar);
                window.display();
            }
        }
    };

    int main() {
        MusicAndVideoPlayer player;
        player.run();

        return 0;
    }
