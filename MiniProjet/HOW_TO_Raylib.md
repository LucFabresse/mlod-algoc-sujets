# Sous Linux

## Compilation manuelle de raylib en version statique

```
$ cd
$ pwd
/home/bot
$ git clone https://github.com/raysan5/raylib.git Raylib.git
$ cd Raylib.git/src
$ make PLATFORM=PLATFORM_DESKTOP
$ ls libraylib*
libraylib.a
```
Le fichier `libraylib.a` a été créé.

## Compiler mon projet

```
$ cd <MonGit>/MiniProjet
$ ls
arkanoid.c README.md
$ gcc -o arkanoid arkanoid.c -Wall -std=c11 /home/bot/Raylib.git/src/libraylib.a -lm
$ ls
arkanoid arkanoid.c README.md
```
Le fichier `arkanoid` est créé.

## Analyse des dépendences d'un exécutable

```
$ ldd arkanoid
	linux-vdso.so.1 (0x00007fff9c9bf000)
	libm.so.6 => /usr/lib/libm.so.6 (0x00007fa2bb87c000)
	libc.so.6 => /usr/lib/libc.so.6 (0x00007fa2bb695000)
	/lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007fa2bba7c000)
```

## Exécuter mon projet

```
./arkanoid
```
