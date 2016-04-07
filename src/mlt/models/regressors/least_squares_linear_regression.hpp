#ifndef MLT_MODELS_REGRESSORS_LEAST_SQUARES_LINEAR_REGRESSION_HPP
#define MLT_MODELS_REGRESSORS_LEAST_SQUARES_LINEAR_REGRESSION_HPP

#include <Eigen/Core>

#include "linear_regressor_model.hpp"
#include "../../utils/linear_solvers.hpp"

namespace mlt {
namespace models {
namespace regressors {
	template <class Solver = utils::linear_solvers::SVDSolver>
    class LeastSquaresLinearRegression : public LinearRegressorModel {
    public:         
        explicit LeastSquaresLinearRegression(bool fit_intercept = true) : LinearRegressorModel(fit_intercept), _solver(Solver()) {}

		explicit LeastSquaresLinearRegression(const Solver& solver, bool fit_intercept = true) : LinearRegressorModel(fit_intercept), _solver(solver) {}

		explicit LeastSquaresLinearRegression(Solver&& solver, bool fit_intercept = true) : LinearRegressorModel(fit_intercept), _solver(solver) {}

        LeastSquaresLinearRegression& fit(const Eigen::MatrixXd& input, const Eigen::MatrixXd& target) {
            Eigen::MatrixXd input_prime(input.rows() + (_fit_intercept ? 1 : 0), input.cols());
			input_prime.topRows(input.rows()) << input;

			if (_fit_intercept) {
				input_prime.bottomRows<1>() = Eigen::VectorXd::Ones(input.cols());
			}

			Eigen::MatrixXd coeffs = _solver.compute(input_prime * input_prime.transpose()).solve(input_prime * target.transpose()).transpose();

			if (_fit_intercept) {
				_set_coefficients_and_intercepts(coeffs.leftCols(coeffs.cols() - 1), coeffs.rightCols<1>());
			}
			else {
				_set_coefficients(coeffs);
			}

			return *this;
        }
	protected:
		Solver _solver;
    };
}
}
}
#endif