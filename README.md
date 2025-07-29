# Brick Breaker Console Game 🎮

A classic **brick breaker arcade game**, built in pure **C++** using console graphics. The player controls a platform at the bottom of the screen to reflect the ball and break bricks. 
The game supports multiple levels, increasing difficulty, and keeps score.

---

## 📌 Features

- Smooth **console animation**
- Ball & paddle physics with **real-time collision detection**
- Multiple **levels** with changing layouts
- **Score tracking** and level-up system
- Simple and clean **UI using ASCII**
- Written in **standard C++**, no external libraries

---

## 🎮 How to Play

- Use `A` and `D` keys to move the platform **left** and **right**
- The ball bounces off walls and the paddle
- Break all the bricks to pass the level
- If the ball falls below the paddle — **game over**

---

## 🛠 Technologies Used

- **C++**
- Console manipulation with `<windows.h>`
- Basic object-oriented design: `Ball`, `Paddle`, `Brick`, `GameManager` classes

---

## 📁 Project Structure

```
/BrickBreaker
├── main.cpp
├── ball.cpp / ball.h
├── paddle.cpp / paddle.h
├── brick.cpp / brick.h
├── game.cpp / game.h
└── utils.cpp / utils.h
```

---

## ✅ Example Screenshot

```
######################################
#                                    #
#  [#][#][#][#][#][#][#][#][#][#]    #
#                                    #
#                                    #
#                 O                  #
#                                    #
#             ===                    #
######################################
Score: 350       Level: 2
```

---

## 🚀 Installation & Run

1. Clone the repository:

```bash
git clone https://github.com/your-username/brick-breaker-game.git
cd brick-breaker-game
```

2. Compile the project (Windows):

```bash
g++ main.cpp ball.cpp paddle.cpp brick.cpp game.cpp utils.cpp -o BrickBreaker.exe
```

3. Run:

```bash
./BrickBreaker.exe
```

---

## 🧠 Future Improvements

- Add **sound effects** using Windows API
- Save/load **high scores**
- Support for **pause/resume**
- Port to **cross-platform** (e.g. using SDL or ncurses)

---

---

## 🎮 Author

 ## Maslukov Ivan

---

## 📜 License

MIT License — free to use, modify, and distribute.
