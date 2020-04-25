# Manual installation

This documentation should work on debian based linux distributions and on windows (I would include others but I am not familiar with them feel free to add them to the scripts).

If you are on windows install [chocolatey](https://chocolatey.org/).
You can execute all commands and scripts in the powershell without the need for another shell, however you can use git bash, mingw, etc.

* Install [git lfs](https://git-lfs.github.com/)
* Then clone the repository (`git clone https://github.com/noah1510/greenfoot-.git`)
* Then change into the directory of the project (`cd "greenfoot-"`)
* Run the build script with the init flag (`./scripts/build.sh --init`)

If you want to you can build the testgame using the following command: `./testgame/scripts/execute.sh --init`

Now you can continue with [customizing the build](https://github.com/noah1510/greenfoot-/wiki/Build-script), [contribute to further development]() or [start making your own game]().
