function [J, grad] = costFunctionReg(theta, X, y, lambda)
%COSTFUNCTIONREG Compute cost and gradient for logistic regression with regularization
%   J = COSTFUNCTIONREG(theta, X, y, lambda) computes the cost of using
%   theta as the parameter for regularized logistic regression and the
%   gradient of the cost w.r.t. to the parameters. 

% Initialize some useful values
m = length(y); % number of training examples

% You need to return the following variables correctly 
J = 0;
grad = zeros(size(theta));
n = size(theta);

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the cost of a particular choice of theta.
%               You should set J to the cost.
%               Compute the partial derivatives and set grad to the partial
%               derivatives of the cost w.r.t. each parameter in theta

% calculate Z as theta' * x ==> in vectorized form, it is X * theta of form m * 1
z = X * theta;
% calculate sigmoid/logistic function (prediction hypothesis) as 1 / ( 1 + e power -z) ==> in vectorized form, it is X * theta of form m * 1
prediction_h = 1./(1 + exp(-z));

% calculate the cost now with the regularization
sum_of_theta_sqr = sum(theta.^2);
% for regularization factor, we dont include theta_0, which is theta(1) in this case
sum_of_theta_sqr = sum_of_theta_sqr - (theta(1) ^ 2);
reg_factor = ((lambda / (2 * m)) * sum_of_theta_sqr);
J = ((-1 / m) * sum(y' * log(prediction_h) + (1 - y)' * log(1 - prediction_h))) + reg_factor;

% Compute the error fraction (h(x) - y)
error_fraction = prediction_h - y;
% Compute lambda / m
l_by_m = lambda / m;
% Now compute gradient factor as a vector
for j = 1:n
    grad(j) = (1 / m) * (sum(error_fraction' * X(:, j)));
    if j > 1
        grad(j) = grad(j) + (l_by_m * theta(j));
    end
end

% =============================================================

end
