function [theta, J_history] = gradientDescent(X, y, theta, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESCENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
m = length(y); % number of training examples
J_history = zeros(num_iters, 1);

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step on the parameter vector
    %               theta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the cost function (computeCost) and gradient here.
    %

    % Compute the prediction of hypothesis
    prediction_h = (X * theta);
    % Compute the error fraction
    error_fraction = prediction_h - y;
    
    % Compute theta0 and theta1 simulataneously in temporary location
    temp_theta0 = theta(1) - (alpha * (1 / m) * (error_fraction' * X(:, 1)));
    temp_theta1 = theta(2) - (alpha * (1 / m) * (error_fraction' * X(:, 2)));

    % Update the theta0 and theta1 values now
    theta(1) = temp_theta0;
    theta(2) = temp_theta1;

    % ============================================================

    % Save the cost J in every iteration    
    J_history(iter) = computeCost(X, y, theta);

end

end
