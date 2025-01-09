{ stdenv, ncurses }:
  stdenv.mkDerivation rec {
    name = "simpleboard";

    nativeBuildInputs = [
      ncurses
    ];

    src = ./.;

    installFlags = [ "PREFIX=$(out)" ];
  }
