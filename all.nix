{ runCommand, self }:

runCommand "all" {} ''
  mkdir $out
  cd $out
  mkdir linux win
  cp ${self.packages.x86_64-linux._linux}/bin/* linux/
  cp -L ${self.packages.x86_64-linux._windows}/bin/* win/
''
