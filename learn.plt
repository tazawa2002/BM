set term pdfcairo size 12in, 8in font "VL-PGothic-Regular.ttf,24"
set output "./graph/learn.pdf"

# 縦軸の最大値を求める処理
stats './graph/bm1_p.txt' using 2
ymax1 = STATS_max
x = STATS_records

stats './graph/bm2_q.txt' using 2
ymax2 = STATS_max

stats './graph/bm1_q.txt' using 2
ymax3 = STATS_max

stats './graph/bm2_p.txt' using 2
ymax4 = STATS_max

ymax = ymax1 > ymax2 ? ymax1 : ymax2
ymax = ymax > ymax3 ? ymax : ymax3
ymax = ymax > ymax4 ? ymax : ymax4

unset key
set multiplot layout 2,2
set yrange [0:ymax*1.1]
set xrange [-0.5:x-0.5]

set title "生成モデルの確率分布"
plot "./graph/bm1_p.txt" smooth freq with boxes fill solid

set title "学習モデルの確率分布"
plot "./graph/bm2_p.txt" smooth freq with boxes fill solid

set title "生成モデルのサンプリング結果"
plot "./graph/bm1_q.txt" smooth freq with boxes fill solid
# plot "./data/data.dat" smooth freq with boxes fill solid

set title "学習モデルのサンプリング結果"
plot "./graph/bm2_q.txt" smooth freq with boxes fill solid

unset multiplot

set term pdfcairo size 6in, 4in font "VL-PGothic-Regular.ttf,24"
set output "./graph/learn2.pdf"

set key default
set title "生成モデルと学習モデルの比較"
plot "./graph/bm1_p.txt" w l title "生成モデルの確率分布", "./graph/bm2_q.txt" w l title "学習モデルのサンプリング結果"