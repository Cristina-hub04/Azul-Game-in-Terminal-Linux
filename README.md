# Azul-Game-in-Terminal-Linux
A Linux terminal version of the popular board game Azul coded in C! This terminal adaptation captures the strategy and aesthetics of the board game, allowing you to compete against a friend or a computer in a simple text-based environment.

Game Overview
Azul is a tile-placement game where players score points by creating patterns with tiles on their boards. Players select tiles from various factories each turn and place them to maximize points while avoiding penalties for unused tiles. This version follows the classic Azul rules and is designed for 1-2 players.

Installation Requirements
Linux
GCC compiler

Getting Started

After launching, the game will prompt you to:

    Select the number of players (1-2).
    Choose the game mode (easy, normal, or hard).

Once setup is complete, the board is displayed, showing tile factories, your pattern lines, and the scoring board. Players take turns selecting and placing tiles to earn points.
Game Rules

    Tile Selection:
        Choose all tiles of one color from any factory or from the center.
        Tiles left over from factories are moved to the center.

    Tile Placement:
        Place chosen tiles on one of five pattern rows.
        Only one color is allowed per row, and it must match existing tiles if any are present.

    Scoring:
        Points are awarded based on tiles in completed rows, columns, and patterns.
        Additional bonuses are given for completing full rows, columns, or colors.

    Penalties:
        Unused tiles go to the floor line, resulting in negative points.

    End of Game:
        The game ends when at least one row is fully filled.
        The player with the highest score wins after final scoring.
