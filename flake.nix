{
  description = "A Super Mario 64 PC port hack with speedrun practicing features.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { self
    , nixpkgs
    , flake-utils
    }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
        # Our packages require proprietary assets and therefore have a non-free license, all dependencies are free.
        config.allowUnfree = true;
      };

      sm64ex-practice = pkgs.callPackage
        ({ lib
         , stdenv
         , pkg-config
         , hexdump
         , python3
         , SDL2
         , audiofile
         , requireFile
         , rom ? throw "sm64ex-practice: no ROM specified, unable to build"
         }:
          stdenv.mkDerivation {
            pname = "sm64ex-practice";
            version =
              let
                year = lib.substring 0 4 self.lastModifiedDate;
                month = lib.substring 4 2 self.lastModifiedDate;
                day = lib.substring 6 2 self.lastModifiedDate;
              in
              "${year}-${month}-${day}";

            src = lib.cleanSource self;

            nativeBuildInputs = [
              pkg-config
              hexdump
              python3
            ];

            buildInputs = [
              SDL2
              audiofile
            ];

            makeFlags = [ "VERSION=${rom.region}" ];

            # Required for linking the dynos patch
            NIX_LDFLAGS = [ "-lstdc++" ];

            enableParallelBuilding = true;

            preBuild = ''
              patchShebangs extract_assets.py
              ln -s ${rom.file} ./baserom.${rom.region}.z64
            '';

            installPhase = ''
              runHook preInstall

              mkdir -p $out/bin
              cp build/${rom.region}_pc/sm64.${rom.region}.f3dex2e $out/bin/sm64ex-practice

              runHook postInstall
            '';

            passthru = {
              inherit rom;
            };

            meta = with lib; {
              description = "A ${rom.region} Super Mario 64 PC port hack with speedrun practicing features";
              longDescription = ''
                You must supply a baserom for the '${region.rom}' version of the game to extract assets from,
                as this contains copyrighted assets. If your copy of the game is from another region you must
                choose the corresponding package from this flake.
              '';
              mainProgram = "sm64ex-practice";
              license = licenses.unfree;
              platforms = platforms.linux;
            };
          })
        { };

      mkRegion = region:
        let
          rom = {
            inherit region;

            file = pkgs.requireFile {
              name = "baserom.${region}.z64";

              message = ''
                This nix expression requires that baserom.${region}.z64 is already part of the store.
                To get this file you can dump your '${region}' region Super Mario 64 cartridge's contents,
                and add it to the nix store with `nix-store --add-fixed sha256 baserom.${region}.z64`.
                If your copy of the game is from another region you must choose the corresponding package from this flake.

                After you copied the ROM to the store you will be able to build this package. Note that without
                a GC-root the file will be removed from the store after a garbage collection, to circumvent
                this you can generate a link to it: `nix build .#sm64ex-practice-${region}.rom.file -o ./rom`
              '';

              sha256 = {
                us = "17ce077343c6133f8c9f2d6d6d9a4ab62c8cd2aa57c40aea1f490b4c8bb21d91";
                eu = "c792e5ebcba34c8d98c0c44cf29747c8ee67e7b907fcc77887f9ff2523f80572";
                jp = "9cf7a80db321b07a8d461fe536c02c87b7412433953891cdec9191bfad2db317";
              }.${region} or (throw "sm64ex-practice: invalid region '${region}'");
            };
          };
        in
        sm64ex-practice.override { inherit rom; };

      sm64ex-practice-us = mkRegion "us";
      sm64ex-practice-jp = mkRegion "jp";
      sm64ex-practice-eu = mkRegion "eu";
    in
    {
      packages = {
        default = sm64ex-practice-us;
        inherit sm64ex-practice-us sm64ex-practice-jp sm64ex-practice-eu;
      };
    });
}
