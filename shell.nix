{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    pkg-config
    python3
    hexdump
  ];

  buildInputs = with pkgs; [
    SDL2
    audiofile
  ];

  NIX_LDFLAGS = [ "-lstdc++" ];
}
