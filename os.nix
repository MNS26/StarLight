{ runCommand, self }:

{
 all =  runCommand "all" {} ''
  mkdir $out
  cd $out
  mkdir linux arm win
  cp -L ${self.packages.x86_64-linux._linux}/* linux/
  cp -L ${self.packages.x86_64-linux._windows}/* win/
  cp -L ${self.packages.x86_64-linux._arm}/* arm/
  '';

 windows =  runCommand "windows" {} ''
  mkdir $out
  cd $out
  mkdir win
  cp -L ${self.packages.x86_64-linux._windows}/* win/
  '';

 linux =  runCommand "linux" {} ''
  mkdir $out
  cd $out
  mkdir linux
  cp ${self.packages.x86_64-linux._linux}/* linux/
  '';

 arm =  runCommand "arm" {} ''
  mkdir $out
  cd $out
  mkdir arm
  cp -L ${self.packages.x86_64-linux._arm}/* arm/
  '';
}