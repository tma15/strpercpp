# strperc++
A C++ implementation of structured perceptron

## install
```sh
autoreconf -iv
./configure --prefix=/path/to/install
make
make install
```

## data format
```
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
…

```

## example
```sh
git clone https://github.com/neubig/nlptutorial.git ~/nlpdata
python convert_data_format.py < ~/nlptutorial/data/wiki-en-train.norm_pos > ./convert_data_format.py > train.data
python convert_data_format.py < ~/nlptutorial/data/wiki-en-test.norm_pos > test.data
```

```sh
train_strpercpp train.data template model
test_strpercpp model test.data
```
