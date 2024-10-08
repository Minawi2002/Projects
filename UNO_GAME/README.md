# Uno

This is a simple C++ implementation of the game "Uno" by MATTEL. You can read the game's rules [here](https://www.unorules.com/). The implementation features a client/server architecture for multiplayer scenarios.
It uses [wxWidgets](https://www.wxwidgets.org/) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests.

## UNORGANIZED

The team is:

- Mahmoud El Minawi
- Sophia Esser
- Michael Klein
- Sebastian Lochmann
- Severin Nigg
- Ielizaveta Polupanova

![Uno-logo](./assets/uno_logo.png?raw=true)

This Code was adapted from a template for the course Software Engineering

## 1. Compile instructions

This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu](https://ubuntu.com/#download), as it offers the easiest way to setup wxWidgets. Thus, the rest of this README assumes you are using Ubuntu. The following was tested on a Ubuntu 22.04 system, but should also work for earlier versions of Ubuntu.

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) hard drive space.

### 1.1 Prepare OS Environment

#### Ubuntu 22.04

The OS should already have git installed. If not, you can use: `sudo apt-get install git`.
**Note:** it might promt you to run `sudo apt-get update` first, depending on which version you installed.

### 1.2 Cloning and Compiling the Repo

1. Use `git clone` to fetch this repository.
2. Then navigate to the unorganized folder in your terminal and execute the build script: `source build.sh`
3. Then reboot the system, as sometimes wxwidgets needs a reboot to work properly.

## 2. Run the Game

1. Navigate to the *unorganized* directory in your console
2. Run the server with `source runserver.sh`. The server will run in the background.
3. Now run x clients (from 1 to 4) by entering `source runclients.sh x` , with a default of 4.
4. If you wish to close everything, run `source kill.sh`

## 3. How to play

You can read the game's rules [here](https://www.unorules.com/).

### Using the GUI

In the Login screen, enter your desired name and press Connect. Also you can choose to either have your client in light or in dark mode with the button on the top right.

![Login Screen](./assets/readme1.png?raw=true)  

![Login Screen Dark](./assets/readme1d.png?raw=true)  

Then, one of the players can start the game and choose whether to play with or without the **questionmark-card**. This card acts as a wild card (so it can be played anytime and you must choose a color after playing it) and also collects all the cards from all the players and distributes them evenly among the players.

![Start Game Screen](./assets/readme2d.png?raw=true)  

The game will start automatically, with one player chosen to start first (it displays whose turn it is above your hand). When it is your turn you may play a card (if you have one that can be played), or draw a card (you can also do this if you have cards that are playable). To draw a card, click on the draw pile. When you have one card left, press on the uno button (underneath the color box), otherwise you will be given two cards from the draw pile as a penalty.

![Game Screen](./assets/readme3d.png?raw=true)  

When a one player has no cards left, the round is done and all players must click start round for the next round to start.

![Round End](./assets/readme4d.png?raw=true)  

When one player reaches 500 points, the winner is announced and the clients will close after pressing the Quit button. You can also start a new game with the same people by pressing the start new game button.

![Game End](./assets/readme5d.png?raw=true)  

## 4. Run the tests

To run the tests, navigate to the *unorganized* directory in your console execute: `source test.sh`
The tests are in the *unorganized/unit-tests* folder.

## 5. Documentation

The documentation can be found in the folder docs. We documented with [Doxygen](https://doxygen.nl/) (the result can be found here: [index.html](./docs/html/index.html)) and created a code overview document with the most important files, functions and variables.
