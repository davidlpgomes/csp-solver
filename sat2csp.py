import argparse
import os
import sys


def main(dimacs_file: str, csp_output: str) -> None:
    """Convert SAT/DIMACS format to CSP"""
    if not os.path.isfile(dimacs_file):
        print(f'File {dimacs_file} does not exist, aborting...')
        sys.exit(1)

    lines = []

    ignore = ['c', '%', '0']

    with open(dimacs_file, 'r') as file:
        for line in file:
            line = line.rstrip()

            if line == '':
                continue

            if not line.rstrip()[0] in ignore:
                lines.append(line)

    header = lines[0].split()
    num_vars = int(header[2])
    num_clasules = int(header[3])

    print(f'num_vars={num_vars} num_clasules={num_clasules}')

    clasules = [[int(n) for n in l.split() if n != '0'] for l in lines[1:]]
    print(clasules)

    with open(csp_output, 'w') as file:
        file.write(f'{num_vars}\n')

        for i in range(num_vars):
            file.write('2 0 1\n')

        file.write(f'{num_clasules}\n')

        for i in range(num_clasules):
            file.write('I\n')

            vars = [abs(v) for v in clasules[i]]
            vars.sort()

            file.write(f'{len(vars)} ')

            for j in range(len(vars) - 1):
                file.write(f'{vars[j]} ')
            file.write(f'{vars[len(vars) - 1]}\n')

            file.write('1 ')

            cs = [v for v in clasules[i]]
            cs = sorted(cs, key=abs)

            for j in range(len(cs) - 1):
                v = 0 if cs[j] > 0 else 1
                file.write(f'{v} ')

            v = 0 if cs[len(cs) - 1] > 0 else 1
            file.write(f'{v}\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog='SAT2CSP',
        description='Convert SAT (DIMACS) format to CSP instance')

    parser.add_argument('dimacs_file')
    parser.add_argument('csp_output')

    args = parser.parse_args()

    main(args.dimacs_file, args.csp_output)
