# Welcome to Snake Chaser Game  

A maze navigation game implemented in C, where you navigate through a maze while avoiding a snake to reach the goal.  

---

## **Features**  
- **Maze Navigation**: Navigate through a maze represented by a 2D array.  
- **Player Movement**: Move the player (`^`) using `w` (Up), `a` (Left), `s` (Down), `d` (Right) keys.  
- **Snake Chase**: Avoid the snake (`~`) that moves towards the player.  
- **Goal**: Reach the goal (`X`) to win the game.  
- **Obstacles**: Navigate around strategically placed obstacles (`O`) in the maze.  

---

## **Installation**  

1. Clone the repository:  
   ```bash  
   git clone https://github.com/AyushHL/Snake-Chaser-Game.git  
   ```  

2. Compile the code:  
   ```bash  
   cd Snake-Chaser-Game  
   gcc -o snake_chaser SnakeChaserGame.c  
   ```  

3. Run the game:  
   ```bash  
   ./snake_chaser metadata.txt  
   ```  

Replace `metadata.txt` with your map file containing maze and object locations.  

---

## **Gameplay**  
- Use `w` (Up), `a` (Left), `s` (Down), `d` (Right) keys to move the player (`^`) around the maze.  
- Type `u` to undo the last move.  
- Avoid the snake (`~`) as it tries to chase you.  
- Reach the goal (`X`) to win the game.  

---

## **Files**  
- **SnakeChaserGame.c**: Contains the main game logic and functions.  
- **metadata.txt**: Map file defining maze dimensions and object locations.  
- **README.md**: This file providing information about the game.  

---

## **Tech Stack**  
- **C Programming Language**  
- **File I/O Operations**  
- **Linked List** for game state management  

---

**Contributions are welcome!**  
If you have any ideas, improvements, or bug fixes, feel free to open an issue or submit a pull request.  
