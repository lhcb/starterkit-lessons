from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-i", "--intxt", nargs="+", required=True)
parser.add_argument("-o", "--outtxt", required=True)
args = parser.parse_args()

with open(args.outtxt, "wt") as outf:
    for f in args.intxt:
        with open(f, "rt") as txt:
            outf.write(txt.read()+"\n")
