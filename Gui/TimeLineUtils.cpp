#include "TimeLineUtils.h"

void ticks_size(
        double xmin,              // 表示範囲の最小値
        double xmax,              // 表示範囲の最大値
        double range_units,       // 表示幅（ピクセル単位）
        double min_tick_size_units, // 最小の目盛り間隔（ピクセル単位）
        double *t_size,          // 出力：計算された目盛りの間隔
        bool* half_tick          // 出力：半分の間隔を使うかどうか
        )
{
    // first, compute the size of min_tick_size_units
    double min_tick_size = min_tick_size_units * (xmax - xmin) / range_units;
    int next_p10 = std::ceil( std::log10(min_tick_size) );
    *t_size = ipow(10., next_p10); // ipow(10, next_p10) overflows for next_p10 <= -16
    assert(*t_size / 10 < min_tick_size);
    assert(*t_size >= min_tick_size);
    if (*t_size / 2 >= min_tick_size) {
        *t_size /= 2;
        if (half_tick) {
            *half_tick = true;
        }
    } else if (half_tick) {
        *half_tick = false;
    }
}

void
ticks_bounds(double xmin,
             double xmax,
             double tick_width,
             bool half_tick,
             int tick_max,
             double *offset,
             int* m1,
             int* m2)
{
    *offset = 0.;
    assert(tick_max >= 0);
    if ( (tick_max > 0) && ( (xmin > 0) || (xmax < 0) ) ) {
        const int h = half_tick ? 2 : 1;
        const int mult = h * tick_width * tick_max;
        // make sure offset is outside of the range (xmin,xmax)
        if (mult != 0) {
            *offset = mult * ( (xmin > 0) ? std::floor(xmin / mult) : std::ceil(xmax / mult) );
        } else {
            *offset = 0;
        }
    }
    *m1 = std::ceil( (xmin - *offset) / tick_width );
    *m2 = std::floor( (xmax - *offset) / tick_width );
}

void
ticks_fill(bool half_tick,
           int tick_max,
           int m1,
           int m2,
           std::vector<int>* ticks)
{
    ticks->resize(m2 - m1 + 1);

    // now all ticks can be obtained by dividing tick_largest by 2, then 5, then 2, then 5, etc.
    int tick_size = 1;
    bool multiply_by_two = half_tick;
    while (tick_size <= tick_max) {
        int tick_1 = std::ceil(m1 / (double)tick_size) * tick_size;
        int tick_2 = std::floor(m2 / (double)tick_size) * tick_size;
        for (int tick = tick_1; tick <= tick_2; tick += tick_size) {
            assert( tick - m1 >= 0 && tick - m1 < (int)ticks->size() );
            (*ticks)[tick - m1] = tick_size;
        }
        if (multiply_by_two) {
            tick_size *= 2;
        } else {
            tick_size *= 5;
        }
        multiply_by_two = !multiply_by_two;
    }

    // last, if 0 is within the interval, give it the value tick_max*10
    if ( (m1 <= 0) && (m2 >= 0) ) {
        (*ticks)[-m1] = 10 * tick_max;
    }
}

// compute alpha value for drawing the ticks
double
ticks_alpha(double min,
            double max,
            double val)
{
    assert(val > 0. && min > 0. && max > 0. && max > min);
    const double alpha = sqrt( (val - min) / (max - min) );

    return std::max( 0., std::min(alpha, 1.) );
}
