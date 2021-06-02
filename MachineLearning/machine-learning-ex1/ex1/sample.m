A = [1 2 3; 2 3 4; 6 3 2; 4 5 7]
muA = mean(A)
sub_muA = repmat(muA, size(A,1), 1);
A_norm = A - sub_muA
sigmaA = std(A)

for index = 1:size(A, 2)
  A_norm(:, index) = A_norm(:, index) / sigmaA(index);
end
A_norm
