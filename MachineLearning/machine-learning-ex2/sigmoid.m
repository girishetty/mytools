function g = sigmoid(z)
%SIGMOID Compute sigmoid function
%   g = SIGMOID(z) computes the sigmoid of z.

% You need to return the following variables correctly 
g = zeros(size(z));

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the sigmoid of each value of z (z can be a matrix,
%               vector or scalar).

% calculate sigmoid/logistic function (prediction hypothesis) as 1 / ( 1 + e power -z) ==> in vectorized form, it is X * theta of form m * 1
g = 1./(1 + exp(-z));


% =============================================================

end
