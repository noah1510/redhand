
<h1 align="center"> <img src="https://github.com/noah1510/redhand/blob/dev-noa/logo/redhand.svg" alt="logo" height="40pt" /> Welcome to RedHand  </h1>

<h2 align="center">
<a href="https://github.com/noah1510/redhand/blob/main/License"><img src="https://img.shields.io/github/license/noah1510/redhand.svg" alt="GitHub license" /></a>
<img src="https://github.com/noah1510/redhand/workflows/CI/badge.svg" alt="build status" />
<a href="https://frontend.code-inspector.com/project/5322/dashboard"><img src="https://www.code-inspector.com/project/5322/score/svg" alt="code quality" /></a>
<a href="https://matrix.to/#/+redhand:tchncs.de"><img src="https://matrix.to/img/matrix-badge.svg" alt="Matrix" /></a>
<a href="https://GitHub.com/noah1510/redhand/releases/"><img src="https://img.shields.io/github/v/release/noah1510/redhand.svg" alt="stable release" /></a>
</h2>

The goal is to create an easy to use 2D Game engine using C++.
Its purpose is to give new programmers an easy introduction into game programming and even more abstract topics like object oriented programming and graphics programming in general.
The used OpenGL APIs are GLFW and glad so this project should run on all operating systems.
**This Project is still in the alpha phase and there might be drastic changes to the codebase and API causing your Programs to break with an update.
Backwards compatibility will be a consideration after the release of version 1.0.0 everything prior to that might include drastic changes!**
This Project provides several functions and objects that make it easier to start building your own games.
While these objects give you a simple way to get started, they also allow you to insert advanced OpenGL commands.

You can find all the documentation [here](https://noah1510.github.io/redhand/).

## License information

The code is licensed under the LGPL-3.0 License.
All the other stuff, like Textures, Images and Sounds are Licensed under CC-0.
You can find the full information [here](https://github.com/noah1510/redhand/blob/main/debian/copyright).

## How to compile/install this project

There are two release types tagged and latest.
The tagged version is generally more stable and tested while the latest version has all the features but might break some games.

### Ubuntu (focal/groovy) based distributions

There are two personal package archieves you can choose from:

* [libredhand tagged](https://launchpad.net/~noasakurajin/+archive/ubuntu/libredhand)
* [libredhand latest](https://launchpad.net/~noasakurajin/+archive/ubuntu/libredhand-latest)

Please note that I don't actively suport ubuntu bionic or even other linux distros.
This project should compile on all those platforms just fine if you figure the dependencies out (and for ubuntu add repositories for newer compiler versions).

### Windows

For an mostly automated install check the [manual Installation](https://noah1510.github.io/redhand/md_doc_markdown_install.html).

You can download the either the latest or the last tagged realese form [github](https://github.com/noah1510/redhand/releases).
The dlls are automatically compiled and uploaded on every tagged and versioned commit.
You can just include those dlls and the headers to you project to build your program using redhand.
You might also need to install glfw to get it working properly
**Make sure your C++ compiler supports the `<filesystem>` include (gcc > 9.2.0 or llvm > 9) since redhand needs it**

### Manually compiling the project

* [Installation](https://noah1510.github.io/redhand/md_doc_markdown_install.html)
* [Documentaion of the code](https://noah1510.github.io/redhand/namespaceredhand.html)

### Extra Notes for contibutors

All changes have to be made on a dev branch and merged to main via a pull request.

If you want to contribute, first fork the project.
After that you can make all the changes you want on your fork.
If you think your changes are ready for the main repository open a pull request to the **dev-noa** branch not main.
The CI has to succeed in compiling before you are allowed to push your changes.
dev-noa is merged into master on a regular basis, when the changes seem stable.

## Getting started

If you want to get started using this project, clone the testgame somewhere and try to understand what the code in src does.
You can safely ignore src/main.cpp since that code is mostly glue to setup redhand and will be mostly the same for every project.

## How to contribute to this project

Just open a new issue if you find a bug or miss a feature, just make shure to use the appropiate tags.

If you have implemented a new feature or bugfix just make a pull request and wait till I have time to watch it.
Please document your changes, this will mak it easier for everyone.

Also feel free to propose updates/changes for the wiki to improve the overall documentation and allow newcomers to have more fun (because they don't have to guess what the functions and objects are doing).
Note for potential developers: please send pull-request to the dev-noa branch.
