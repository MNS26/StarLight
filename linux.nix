{ runCommand, self }:

runCommand "linux" {} ''
  mkdir $out
  cd $out
  mkdir linux
  cp ${self.packages.x86_64-linux._linux}/* linux/
''
