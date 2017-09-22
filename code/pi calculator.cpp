#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/progress.hpp>

#include <cmath> //required by std::sqrt/std::log

//If async is not defined, the program is single-threaded.
#define ASYNC

#ifdef ASYNC
#include <future>
#include <atomic>
#endif


namespace nine_digits_of_Pi
{
	/*
	* Computation of the n'th decimal digit of pi with very little memory.
	* Written by Fabrice Bellard on January 8, 1997.
	* Ported to C# by Chris Sells on May 5, 2002.
	*
	* We use a slightly modified version of the method described by Simon
	* Plouffe in "On the Computation of the n'th decimal digit of various
	* transcendental numbers" (November 1996). We have modified the algorithm
	* to get a running time of O(n^2) instead of O(n^3log(n)^3).
	*
	* This program uses mostly integer arithmetic. It may be slow on some
	* hardwares where integer multiplications and divisons must be done
	* by software.
	*
	* Ported to C++ by Jinhao on September 17, 2017.
	*/

	inline int mul_mod(long a, long b, int m)
	{
		return (int)((a * b) % m);
	}

	// return the inverse of x mod y
	int inv_mod(int x, int y)
	{
		int u = x;
		int v = y;
		int a = 0;
		int c = 1;

		do
		{
			auto const q = v / u;

			auto t = c;
			c = a - q*c;
			a = t;

			t = u;
			u = v - q*u;
			v = t;
		} while (u != 0);

		a = a%y;
		if (a < 0) a = y + a;

		return a;
	}

	// return (a^b) mod m
	int pow_mod(int a, int b, int m)
	{
		int r = 1;
		int aa = a;

		while (true)
		{
			if ((b & 0x01) != 0) r = mul_mod(r, aa, m);

			b = b >> 1;
			if (b == 0) break;
			aa = mul_mod(aa, aa, m);
		}

		return r;
	}

	// return true if n is not prime
	bool not_prime(int n)
	{
		if ((n & 1) == 0) return true;

		int r = (int)(std::sqrt(n));
		for (int i = 3; i <= r; i += 2)
		{
			if ((n % i) == 0) return true;
		}

		return false;
	}

	// return the prime number immediately after n
	int next_prime(int n)
	{
		while (not_prime(++n));

		return n;
	}

	int at(int n)
	{
		auto N = static_cast<const int>((n + 20)* std::log(10) / std::log(2));
		int t = 0;
		double sum = 0.0;

		for (int a = 3; a <= (2 * N); a = next_prime(a))
		{
			auto const vmax = static_cast<int>(std::log(2 * N) / std::log(a));

			auto av = 1;

			for (int i = 0; i < vmax; ++i)
				av *= a;

			int s = 0;
			int num = 1;
			int den = 1;
			int v = 0;
			int kq = 1;
			int kq2 = 1;

			for (int k = 1; k <= N; ++k)
			{
				t = k;
				if (kq >= a)
				{
					do
					{
						t = t / a;
						--v;
					} while ((t % a) == 0);

					kq = 0;
				}

				++kq;
				num = mul_mod(num, t, av);

				t = (2 * k - 1);
				if (kq2 >= a)
				{
					if (kq2 == a)
					{
						do
						{
							t = t / a;
							++v;
						} while ((t % a) == 0);
					}

					kq2 -= a;
				}

				den = mul_mod(den, t, av);
				kq2 += 2;

				if (v > 0)
				{
					t = inv_mod(den, av);
					t = mul_mod(t, num, av);
					t = mul_mod(t, k, av);
					for (int i = v; i < vmax; ++i) t = mul_mod(t, a, av);
					s += t;
					if (s >= av) s -= av;
				}
			}

			t = pow_mod(10, n - 1, av);
			sum = std::fmod((sum + mul_mod(s, t, av) / (double)av), 1.0);
		}

		return static_cast<int>(sum * 1e9);
	}
};

#ifdef ASYNC
template<typename Progress>
std::string calc_pi(std::size_t digits, Progress progress)
{
	std::string pi = "3.";
	for (std::size_t i = 0; i < digits; i += 9) {
		int nine_digits = nine_digits_of_Pi::at(i + 1);
		auto count = (std::min)(digits - i, static_cast<std::size_t>(9));

		pi += std::to_string(nine_digits).substr(0, count);

		//Stop calculating if progress returns false
		if (!progress(i + count))
			return{};
	}

	return pi;
}
#else
template<typename Progress>
std::string calc_pi(std::size_t digits, Progress progress)
{
	std::string pi = "3.";
	for (std::size_t i = 0; i < digits; i += 9) {
		int nine_digits = nine_digits_of_Pi::at(i + 1);
		auto count = (std::min)(digits - i, static_cast<std::size_t>(9));

		pi += std::to_string(nine_digits).substr(0, count);

		progress(i + count);
	}

	return pi;
}
#endif


int main()
{
	using namespace nana;

	form fm;
	fm.caption("Pi Calculator");

	label lab{ fm, "Digits of Pi:" };
	lab.text_align(align::right, align_v::center);
	
	spinbox spin{fm};
	spin.range(1, 9000000, 1);

	button btn{ fm };
	btn.caption("Calculate");

	textbox txt{ fm };
	txt.editable(false);
	txt.enable_caret();
	txt.line_wrapped(true);

	progress prg{ fm };

#ifdef ASYNC
	std::future<void> pi_future;

	//A flag indicates the form is unloading.
	std::atomic<bool> unloaded{ false };

	btn.events().click([&]{
		//The button is used to check the status of calculation
		//in unload event handler.
		btn.enabled(false);

		pi_future = std::async(std::launch::async, [&]{
			auto digits = spin.to_int();

			prg.amount(digits);
			prg.value(0);
			auto pi = calc_pi(digits, [&](std::size_t calculated_digits){
				prg.value(calculated_digits);

				return !unloaded.load();
			});

			txt.caption(pi);
			btn.enabled(true);
		});
	});

	fm.events().unload([&](const arg_unload& arg){
		//It indicates the calculation is working if button
		//is disabled.
		if(false == btn.enabled())
		{
			unloaded = true;

			internal_revert_guard irg; //This is important
			pi_future.wait();
		}
	});
#else
	btn.events().click([&]{
		btn.enabled(false);
		auto digits = spin.to_int();

		prg.amount(digits);
		prg.value(0);
		auto pi = calc_pi(digits, [&](std::size_t calculated_digits){
			prg.value(calculated_digits);
		});

		txt.caption(pi);
		btn.enabled(true);
	});
#endif


	fm.div("vert <weight=35 margin=5 arrange=[80,variable, 90] gap=7 conf><text><weight=12 progress>");
	fm["conf"] << lab << spin << btn;
	fm["text"] << txt;
	fm["progress"] << prg;
	fm.collocate();
	
	fm.show();

	exec();
}
