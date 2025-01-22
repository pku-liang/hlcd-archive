import re

def linear(a, b, c, d, x):
    return min((x - a) / (b - a) * (d - c) + c, c)

def compute_score(result):
    check = lambda rule: all([a <= b for a, b in zip(result, rule)])
    scores = [0]
    if check([12000000, 95, 90, 40, 60]):
        scores.append(10)
    if check([1700000, 95, 90, 40, 60]):
        scores.append(20)
    if check([1700000, 30, 90, 25, 50]):
        scores.append(30)
    if check([700000, 30, 15, 10, 15]):
        scores.append(50)
    if check([300000, 30, 10, 10, 10]):
        scores.append(linear(80000, 300000, 80, 60, result[0]))
    if check([80000, 20, 10, 5, 10]):
        scores.append(linear(20000, 80000, 100, 80, result[0]))
    if check([19000, 20, 10, 5, 10]):
        scores.append(100.1) # very close to the theoretical limit
    return max(scores)

def load_line(file: str):
    with open(file) as f:
        inside_roi = False
        for line in f:
            if line.startswith('+ Performance & Resource Estimates:'):
                inside_roi = True
            if line.startswith('='):
                inside_roi = False
            if inside_roi:
                line = line.strip()
                if line.startswith('|+ mm'):
                    return line

def parse_result(file: str):
    line = load_line(file)
    items = line.strip('|').split('|')
    lat = int(items[3])
    def get(s: str):
        return int(re.match(r'\d+\s*\((\d+)%\)', s.strip()).group(1))
    bram = get(items[9])
    dsp  = get(items[10])
    ff   = get(items[11])
    lut  = get(items[12])
    print(f'LATENCY = {lat:5}')
    print(f'BRAM    = {bram:3} %')
    print(f'DSP     = {dsp:3} %')
    print(f'FF      = {ff:3} %')
    print(f'LUT     = {lut:3} %')
    return [lat,  bram, dsp, ff, lut]

def print_score_message(score):
  if score > 100:
    print('You are very close to the theoretical limit!')
    print('  Congratulations on your outstanding performance!')
    print('  Your commitment to HLS is truly admirable!')
    print('  I believe you have understood the principles and methods of hardware acceleration.')
  elif score == 100:
    print('Oh, you made it, Congratulations!')
    print('  Keep up the fantastic work!')
    print('  I believe you have understood the principles and methods of hardware acceleration.')
  elif score >= 80:
    print('Ok, you are almost there, however, the final performance gap is not a easy trial to optimize!')
    print('  Adjust your parameters, or some axi options and optimize!')
  elif score >= 60:
    print('You are getting on the right way! Keep in mind:')
    print('  How many multiplication operation is need?')
    print('  How many multipliers can we use?')
    print('  How to ensure the multipliers are pipelined every cycle?')
  else:
    print('Some fundamental problems in your program, try to fix them!')
    print('  Is there something wrong with the c-simulation?')
    print('  Is the data type floating, not fixed?')
    print('  Is too many unroll or too wrong pragma?')

if __name__ == '__main__':
    vec = parse_result('work/reports/hls_compile.rpt')
    score = compute_score(vec)
    print(f'SCORE   = {min(score, 100):5.1f}')
    print()
    print_score_message(score)
