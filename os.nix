{ runCommand, self }:

let
  # copy a single platform output into a dir named <dir>
  wrap = name: dir: variant:
    let attr = "_${name}-${variant}"; in
    runCommand "${name}-${variant}" {} ''
      mkdir $out
      cd $out
      mkdir ${dir}
      cp -L ${self.packages.x86_64-linux.${attr}}/* ${dir}/
    '';

  # combine all three platforms for a given variant
  combine = variant:
    let
      linux = self.packages.x86_64-linux.${"_linux-${variant}"};
      windows = self.packages.x86_64-linux.${"_windows-${variant}"};
      arm = self.packages.x86_64-linux.${"_arm-${variant}"};
    in
    runCommand "all-${variant}" {} ''
      mkdir $out
      cd $out
      mkdir linux arm win
      cp -L ${linux}/* linux/
      cp -L ${windows}/* win/
      cp -L ${arm}/* arm/
    '';
in
rec {
  linux-debug = wrap "linux" "linux" "debug";
  linux-release = wrap "linux" "linux" "release";
  windows-debug = wrap "windows" "win" "debug";
  windows-release = wrap "windows" "win" "release";
  arm-debug = wrap "arm" "arm" "debug";
  arm-release = wrap "arm" "arm" "release";

  all-debug = combine "debug";
  all-release = combine "release";

  # backwards-compatible aliases (default to debug)
  linux = linux-debug;
  windows = windows-debug;
  arm = arm-debug;
  all = all-debug;
}
