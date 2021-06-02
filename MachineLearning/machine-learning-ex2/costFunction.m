function [J, grad] = costFunction(theta, X, y)
%COSTFUNCTION Compute cost and gradient for logistic regression
%   J = COSTFUNCTION(theta, X, y) computes the cost of using theta as the
%   parameter for logistic regression and the gradient of the cost
%   w.r.t. to the parameters.

% Initialize some useful values
[m, n] = size(X); % number of training examples and no of varibles

% You need to return the following variables correctly 
J = 0;
grad = zeros(size(theta));
n = size(theta);

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the cost of a particular choice of theta.
%               You should set J to the cost.
%               Compute the partial derivatives and set grad to the partial
%               derivatives of the cost w.r.t. each parameter in theta
%
% Note: grad should have the same dimensions as theta
%

% calculate Z as theta' * x ==> in vectorized form, it is X * theta of form m * 1
z = X * theta;
% calculate sigmoid/logistic function (prediction hypothesis) as 1 / ( 1 + e power -z) ==> in vectorized form, it is X * theta of form m * 1
prediction_h = 1./(1 + exp(-z));

% calculate the cost now
J = (-1 / m) * sum(y' * log(prediction_h) + (1 - y)' * log(1 - prediction_h));

% Compute the error fraction (h(x) - y)
error_fraction = prediction_h - y;
% Now compute gradient factor as a vector
for j = 1:n
    grad(j) = (1 / m) * (sum(error_fraction' * X(:, j)));
end

% =============================================================

end
