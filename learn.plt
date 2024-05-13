set term pdfcairo size 12in, 8in font "VL-PGothic-Regular.ttf,24"
set output "./graph/learn.pdf"

unset key
set multiplot layout 2,2

set title "学習元の確率分布"
plot "./graph/bm1_p.txt" smooth freq with boxes fill solid title "学習元"

set title "学習先の確率分布"
plot "./graph/bm2_p.txt" smooth freq with boxes fill solid title "学習先"

set title "学習元の経験分布"
plot "./graph/bm1_q.txt" smooth freq with boxes fill solid title "学習元"

set title "学習先の経験分布"
plot "./graph/bm2_q.txt" smooth freq with boxes fill solid title "学習先"

unset multiplot

set term pdfcairo size 6in, 4in font "VL-PGothic-Regular.ttf,24"
set output "./graph/graph2.pdf"

set key default
set title "出現確率と確率分布の比較"
plot "./graph/bm1.txt" w l title "学習元", "./graph/bm2.txt" w l title "学習先"