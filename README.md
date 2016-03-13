# Space Shooter
This is a game created for my Game Engineering Principles midterm assignment.

## Setup
If downloading the repo and running it doesn't work, you may not have SDL set up within Visual Studio.  This project uses:
- `SDL2 (base)`
- `SDL_TTF`
- `SDL_mixer`
- `SDL_image`

Make sure the include paths and libraries are set up in your build properties.

This project is also set up to use [Visual Leak Detector](https://vld.codeplex.com/) to find leaks.  It will run in Debug mode, with output appearing in VS's Output window.

You may also need to add `$(SolutionDir)\include` to Visual Studio's include path list, if this is not configured globally already.

## Controls
- W A S D Movement
- B to show hit**b**oxes
- R to reset/restart game
- Up arrow to increase number of cannons by 1 (only when energy is 100%, will consume all energy)
- Down arrow to reduce number of cannons by 1 (restores all energy instantly)

## Credits
Thank you to each of these resources:

- Lukasz Wawrzyniak, professor
- Wherever [this spaceship](http://static1.squarespace.com/static/50c25455e4b0ef5720704c6b/54b94ba5e4b0b5bb4be6a892/54b94d72e4b0c2c1fae545b6/1421430256503/Spaceship.png) I found on Google Images is from
- [This spatial hashing guide](https://conkerjo.wordpress.com/2009/06/13/spatial-hashing-implementation-for-fast-2d-collisions/) for bringing down this simple technique to an even simpler-minded programmer
- [Spaceship graphic](http://opengameart.org/content/simple-spaceship)
- [Explosion and laser graphics](http://opengameart.org/content/asteroid-explosions-rocket-mine-and-laser)
- [Font](http://www.dafont.com/visitor.font)
