# -*- coding: utf-8 -*-
import sys

n_corr_tok = 0
n_tok = 0

n_sent = 0
n_corr_sent = 0

sent_true = True

for line in sys.stdin:
    elems = line.strip().split('\t')
    if len(elems) != 3:
        n_sent += 1

        if sent_true:
            n_corr_sent += 1

        sent_true = True
        print('\n')
        continue

    print(line.strip())
    token = elems[0]
    true = elems[1]
    pred = elems[2]

    n_tok += 1
    if true == pred:
        n_corr_tok += 1
    else:
        sent_true = False

print('{}/{}'.format(n_corr_tok, n_tok))
print('{}/{}'.format(n_corr_sent, n_sent))
