{ runCommand, self }:

runCommand "windows" {} ''
  mkdir $out
  cd $out
  mkdir win
  cp -L ${self.packages.x86_64-linux._windows}/* win/
''
