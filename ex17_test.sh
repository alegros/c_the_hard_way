set -e
echo _____ Infos _____
./ex17 db.dat i
echo _____ Création 10000 512... _____
./ex17 db.dat c 10000 512
echo _____ Insertions... _____
./ex17 db.dat s 1 Alex alex@gmail.com 0678102587
./ex17 db.dat s 25 Gaëlle gaëlle@gmail.com 0688888888
./ex17 db.dat s 9001 Panda m.panda@gmail.com 06badass!!
./ex17 db.dat s 9002 x x@gmail.com 0600000000
echo _____ List _____
./ex17 db.dat l
echo _____ Suppression x _____
./ex17 db.dat d 9002
echo _____ List _____
./ex17 db.dat l
echo _____ Find Gaëlle _____
./ex17 db.dat f Gaëlle
echo _____ Find x _____
./ex17 db.dat f x
