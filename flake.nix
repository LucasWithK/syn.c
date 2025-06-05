{
  inputs.nixpkgs.url = "nixpkgs/nixpkgs-unstable";

  outputs = { nixpkgs, ... } @ inputs:
    let
      systems = [ "x86_64-linux" ];

      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
      devShells = forAllSystems ({ pkgs }: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            # Add packages
          ];
        };
      });
    };
}
