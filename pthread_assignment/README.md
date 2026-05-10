# PL1

1.1:(μεταγλωτιση και πολλα run)
    make clean
    make all
    ./analysis.sh

1.2:(compile και execute μέσω makefile για 10 νήματα)
    run_atomic
    run_rwlock
    run_mutex

1.3:(μεταγλωτιση και πολλα run)
    make clean
    make all
    ./test.sh

1.4:(compile και execute μέσω makefile)

    run_bank_fine_rw
    run_bank_fine_mutex
    run_bank_coarse_mutex
    run_bank_coarse_rw

    για να δείτε τα ορίσματα στο αντίστοιχο Makefile της 1.4

1.5:(compile και execute μέσω makefile)
    run_barrier_sense_reversal_centralized
    run_barrier_mutex_cond
    run_barrier_pthead

    για διαφορετικό αριθμό νημάτων check makefile

Στον εκάστοτε folder κανείς μπορεί να βρει στον subfolder src το source code των προγραμματων, στο subfolder include τα απαραίτητα .h αρχεία.
Επιπλεόν, σε κάποιος φακέλους δημιουργείται ένα subfolder gitignore αυτό αγνοείστε το απλά εκεί πάνε τα .o αρχεία.
Τέλος, στο folder της κάθε άσκησης υπάρχουν και τα scripts (.sh και .py) που χρησιμοποιήθηκαν για τη συλλογή, αποιήκευση και ανάλυση των αποτελεσμάτων.
    