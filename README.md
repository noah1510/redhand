
<h1 align="center"> <img src="https://github.com/noah1510/redhand/blob/dev-noa/logo/redhand.svg" alt="logo" height="40pt" /> Welcome to RedHand  </h1>

<h2 align="center">
<a href="https://github.com/noah1510/redhand/blob/master/License"><img src="https://img.shields.io/github/license/noah1510/redhand.svg" alt="GitHub license" /></a>
<img src="https://github.com/noah1510/redhand/workflows/CI/badge.svg" alt="build status" />
<a href="https://frontend.code-inspector.com/project/5322/dashboard"><img src="https://www.code-inspector.com/project/5322/score/svg" alt="code quality" /></a>
<a href="https://discord.gg/awjW3aG"><img src="https://discordapp.com/api/guilds/691968013234339881/widget.png" alt="Discord" /></a>
<a href="https://GitHub.com/noah1510/redhand/releases/"><img src="https://img.shields.io/github/v/release/noah1510/redhand.svg" alt="stable release" /></a>
</h2>

The goal is to create an easy to use 2D Game engine using C++.
Its purpose is to give new programmers an easy introduction into game programming and even more abstract topics like object oriented programming and graphics programming in general.
The used OpenGL APIs are GLFW and glad so this project should run on all operating systems.

This Project provides several functions and objects that make it easier to start building your own games.
While these objects give you a simple way to get started, they also allow you to insert advanced OpenGL commands.

You can find all the documentation [here](https://noah1510.github.io/redhand/).

## License information

The code is licensed under the LGPL-3.0 License.
All the other stuff, like Textures, Images and Sounds are Licensed under CC-0.
You can find the full information [here](https://github.com/noah1510/redhand/blob/master/debian/copyright).

## How to compile/install this project

There are two release types tagged and latest.
The tagged version is generally more stable and tested while the latest version has all the features but might break some games.

### Ubuntu (bionic) based distributions

There are two personal package archieves you can choose from:

* [libredhand tagged](https://launchpad.net/~noasakurajin/+archive/ubuntu/libredhand)
* [libredhand latest](https://launchpad.net/~noasakurajin/+archive/ubuntu/libredhand-latest)

### Windows

You can download the either the latest or the last tagged realese form [github](https://github.com/noah1510/redhand/releases).
The dlls are automatically compiled and uploaded on every tagged and versioned commit.

### Manually compiling the project

* [Installation](md_doc_markdown_install.html)
* [Documentaion of the code](namespaceredhand.html)

### Extra Notes for contibutors

All changes have to be made on the dev branch and merged to master via a pull request.
Open a Terminal in the folder where you cloned the repository to, type `git checkout dev` and you are ready to go.
To update the branch from master first type `git fetch` then `git rebase origin/master`.
The CI has to succeed in compiling before you are allowd to push your changes to master.

## Getting started

If you want to get started using this project, begin by modifying the games.cpp file.
The functions that specify the game logic are here.

game_init is called in the beginning to construct the main world that the game will enter first.
game_loop is called every game tick, here is where all the logic happens.

## How to contribute to this project

Just open a new issue if you find a bug or miss a feature, just make shure to use the appropiate tags.

If you have implemented a new feature or bugfix just make a pull request and wait till I have time to watch it.
Please document your changes, this will mak it easier for everyone.

Also feel free to propose updates/changes for the wiki to improve the overall documentation and allow newcomers to have more fun (because they don't have to guess what the functions and objects are doing).
