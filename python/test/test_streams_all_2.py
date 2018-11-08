# -*- coding: utf-8 -*-
import sys
if len(sys.argv) > 2:
    sys.path.insert(0, sys.argv[2])
import hfst_dev
if len(sys.argv) < 1:
    raise RuntimeError('Transducer format must be given as first argument')

if sys.argv[1] == 'sfst':
    if not hfst_dev.HfstTransducer.is_implementation_type_available(hfst_dev.ImplementationType.SFST_TYPE):
        sys.exit(77)
    hfst_dev.set_default_fst_type(hfst_dev.ImplementationType.SFST_TYPE)
elif sys.argv[1] == 'foma':
    if not hfst_dev.HfstTransducer.is_implementation_type_available(hfst_dev.ImplementationType.FOMA_TYPE):
        sys.exit(77)
    hfst_dev.set_default_fst_type(hfst_dev.ImplementationType.FOMA_TYPE)
elif sys.argv[1] == 'openfst':
    if not hfst_dev.HfstTransducer.is_implementation_type_available(hfst_dev.ImplementationType.TROPICAL_OPENFST_TYPE):
        sys.exit(77)
    hfst_dev.set_default_fst_type(hfst_dev.ImplementationType.TROPICAL_OPENFST_TYPE)
else:
    raise RuntimeError('implementation format not recognized')

istr = hfst_dev.HfstInputStream()
transducers = istr.read_all()
istr.close()

if not len(transducers) == 3:
    raise RuntimeError('Wrong number of transducers read.')

i = 0
for re in ['föö:bär','0','0-0']:
    if not transducers[i].compare(hfst_dev.regex(re)):
        raise RuntimeError('Transducers are not equivalent.')
    i += 1

if len(transducers) > 0:
    f = sys.stdout
    i=0
    transducers[i].write_att(f)
    i += 1
    while i < len(transducers):
        f.write('--\n')
        transducers[i].write_att(f)
        i += 1
