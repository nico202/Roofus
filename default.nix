with import <nixpkgs> {}; {
  Roofus = stdenv.mkDerivation {
    name = "Roofus";
    buildInputs = [
        ncurses
        gcc
    ];
 
    
    shellHook = ''
        unset http_proxy
        export LD_LIBRARY_PATH=${ncurses}
    '';
  };
}

