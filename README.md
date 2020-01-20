# Cod Networking
[![License](https://img.shields.io/github/license/codmf/net)](https://github.com/codmf/net/blob/master/LICENSE)
[![Releases](https://img.shields.io/github/v/release/codmf/net?include_prereleases&sort=semver)](https://github.com/codmf/net/releases)
[![Build Status](https://img.shields.io/travis/codmf/net)](https://travis-ci.org/codmf/net)
[![Code Coverage](https://img.shields.io/codecov/c/gh/codmf/net)](https://codecov.io/gh/codmf/net)
[![Codacy Grade](https://img.shields.io/codacy/grade/ad77fe33ca154127bc66980b742db5e0)](https://app.codacy.com/gh/codmf/net/dashboard)

The Cod networking layer is a simple layer that helps us interface with networking.
It provides a simple and easy API which can be implemented to support different environments.

## Purpose
The purose of the Cod networking layer is to provide a simple, consistent API across all platforms.
There are a lot of different API's on Windows, Linux, etc. that would require vastly complicated implementations.
Rather than spending time trying to work all of those into a project, we opted to create a new API to simplify the experience.

## Inspiration
The inspiration behind this project would be the [Cod Microframework](https://github.com/codmf/cod).
Cod is a microframework written in C, and it aims to not rely on any external dependencies.
This project is a member of the [Cod Microframework Ecosystem](https://github.com/codmf).
That means that it is an official project that is supported and used by Cod.

## Current status
Right now, we are still in extremely early development.
Things aren't expected to actually be working for a while.
Sit tight, and check back in a while!

## Usage
This project uses [Meson](https://mesonbuild.com/) as its build system.
Once you have that installed, you have two options.
You could clone this repository to make some changes, or you could use this project as a dependency.

### Making changes
If you want to make changes, first you need to clone this repository.
Once you have the code, you can run the following command to build the project:

```text
meson build
```
This will build the project into the `build` directory.

The rest is a work-in-progress.
Wait for more!

### Using as a dependency
If you want to use this project as a dependency, first you need to have a project.
From there, you can add this repository as a subproject using Meson's [wrap system](https://mesonbuild.com/Wrap-dependency-system-manual.html).
Your wrap file should look something like the following:

```text
[wrap-git]
url = https://github.com/codmf/net.git
revision = head
```

The rest is a work-in-progress.
Wait for more!
