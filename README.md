# Sandlog
 
![Sandlog](gitassets/Sandlog.gif)

Sandlog is a terminal-based logging tool that visualizes tracked effort as sand falling through an ASCII hourglass. Log any quantity in any unit — hours, sessions, days, reps — and watch the sand fall.
 
## Inspiration
 
Steam's playtime tracker is a small but satisfying thing. Seeing a number climb over time adds a quiet sense of progress to something you're already doing. Sandlog is an attempt to bring that feeling to habits outside of gaming — workouts, study sessions, practice hours — with the added wrinkle that you choose the unit, so the number means whatever you want it to mean.
 
## Usage
 
Run the executable and use the following keywords to navigate:
 
| Command | Description |
|---|---|
| `create` | Create a new project |
| `delete` | Delete an existing project |
| `archive` | Archive an existing project |
| `[project name]` | Open a project and log a quantity |
| `main` | Return to the main menu |
| `exit` | Close the application |
 
When creating a project, you'll be prompted for a name, a size (100 or 400 units), and a unit label. Once created, typing the project name from any state will open it for logging.
 
## Requirements
 
**To run a pre-compiled executable**
- Windows
**To build from source**
- C++17 or later
- Windows (or replace `system("cls")` with `system("clear")` for Linux/Mac)
## Notes
 
Single-file C++ source. No external dependencies.
