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

## Examples
### CONLL2000
You can download one of famous benchmark data of chunking as follows:
```sh
$ cd scripts
$ ./download_conll2000.sh
```

Then, a model is learned and an output is obtained as follows:
```sh
$ train_strpercpp -e 3 ./data/train.txt template.conll model
$ test_strpercpp -v model ./data/test.txt > output.txt
```

Evaluation metrics are calculated as follows:

```
$ ./scripts/conlleval -d "        " < output.txt
processed 47377 tokens with 23852 phrases; found: 24240 phrases; correct: 21654.
accuracy:  94.02%; precision:  89.33%; recall:  90.78%; FB1:  90.05
             ADJP: precision:  60.78%; recall:  64.38%; FB1:  62.53  464
             ADVP: precision:  76.93%; recall:  77.02%; FB1:  76.98  867
            CONJP: precision:   0.00%; recall:   0.00%; FB1:   0.00  18
             INTJ: precision:  50.00%; recall:  50.00%; FB1:  50.00  2
              LST: precision:   0.00%; recall:   0.00%; FB1:   0.00  0
               NP: precision:  89.42%; recall:  90.98%; FB1:  90.19  12638
               PP: precision:  95.66%; recall:  95.72%; FB1:  95.69  4814
              PRT: precision:  78.65%; recall:  66.04%; FB1:  71.79  89
             SBAR: precision:  75.00%; recall:  86.92%; FB1:  80.52  620
              UCP: precision:   0.00%; recall:   0.00%; FB1:   0.00  1
               VP: precision:  90.18%; recall:  91.52%; FB1:  90.85  4727
```

Results of existing models are listed in [this page](https://www.clips.uantwerpen.be/conll2000/chunking/).

## References
- Michael Collins, "Discriminative Training Methods
for Hidden Markov Models: Theory and Experiments
with Perceptron Algorithms.", EMNLP, 2002.
- Michael Collins and Brian Roark, "Incremental Parsing with the Perceptron Algorithm", ACL, 2004.
