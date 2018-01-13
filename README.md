# strperc++
A C++ implementation of structured perceptron

## Install
```sh
autoreconf -iv
./configure --prefix=/path/to/install
make
make install
```

## Data format
```
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
…

```

## Example
### Data preparation
```sh
git clone https://github.com/neubig/nlptutorial.git ~/nlpdata
python scripts/convert_data_format.py < ~/nlptutorial/data/wiki-en-train.norm_pos > ./convert_data_format.py > train.data
python scripts/convert_data_format.py < ~/nlptutorial/data/wiki-en-test.norm_pos > test.data
```

### Training and evaluation
```sh
train_strpercpp -e 3 train.data template model
test_strpercpp model test.data
```
