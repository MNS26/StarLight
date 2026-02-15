{ runCommand, self }:

{
 all =  runCommand "all" {} ''
  mkdir $out
  cd $out
  mkdir linux win
  cp ${self.packages.x86_64-linux._linux}/bin/* linux/
  cp -L ${self.packages.x86_64-linux._windows}/bin/* win/
  '';

 windows =  runCommand "windows" {} ''
  mkdir $out
  cd $out
  mkdir win
  cp -L ${self.packages.x86_64-linux._windows}/bin/* win/
  '';
 linux =  runCommand "linux" {} ''
  mkdir $out
  cd $out
  mkdir linux
  cp ${self.packages.x86_64-linux._linux}/bin/* linux/
  '';

}