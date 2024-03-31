[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<br />
<div align="center">
<img src="images/logo.svg" width="300">

<h3 align="center">Blitz Chess</h3>
<p align="center">Blazingly fast chess engine.</p>

  <p align="center">
    <a href="https://github.com/7yrix/blitz-chess/issues">Report Bug</a>
    ·
    <a href="https://github.com/7yrix/blitz-chess/issues">Request Feature</a>
  </p>
</div>

- [About The Project](#about-the-project)
- [Usage](#usage)
- [Building from Source](#building-from-source)
- [Contributing](#contributing)
- [License](#license)


## About The Project

Blitz Chess is a chess engine that is designed to be as fast as possible. It is written in C. The engine is still in development and is not yet ready for use in tournaments.


## Usage

The blitz chess engine comes as a standalone binary. You can run it from the command line. The engine will output the best move it has found for the given position.

```sh
./bin/latest <fen>
```

## Building from Source

To build the engine from source, you will need to have NASM and ld installed on your system. You can then run the following commands to build the engine.

```sh
make build # Build the engine
make run # Run the engine
make test # Run the tests
```

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.


[contributors-shield]: https://img.shields.io/github/contributors/7yrix/blitz-chess.svg?style=for-the-badge
[contributors-url]: https://github.com/7yrix/blitz-chess/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/7yrix/blitz-chess.svg?style=for-the-badge
[forks-url]: https://github.com/7yrix/blitz-chess/network/members
[stars-shield]: https://img.shields.io/github/stars/7yrix/blitz-chess.svg?style=for-the-badge
[stars-url]: https://github.com/7yrix/blitz-chess/stargazers
[issues-shield]: https://img.shields.io/github/issues/7yrix/blitz-chess.svg?style=for-the-badge
[issues-url]: https://github.com/7yrix/blitz-chess/issues
[license-shield]: https://img.shields.io/github/license/7yrix/blitz-chess.svg?style=for-the-badge
[license-url]: https://github.com/7yrix/blitz-chess/blob/master/LICENSE.txt