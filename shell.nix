{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell
{
  nativeBuildInputs = with pkgs; [
    gcc
    gnumake
    ncurses
    glibc
  ];
}

