//! The linalg module provides some basic linear algebra functionality for transforming 3D geometry
//!
use std::f32;
use std::ops::{Index, Mul, Add};

// Re-export the linalg types from the internal modules
pub use self::vec3::Vec3;
pub mod vec3;

/// Compute the cross product of two vectors
pub fn cross<A: Index<usize, Output = f32>, B: Index<usize, Output = f32>>(
    a: &A,
    b: &B,
) -> vec3::Vec3 {
    Vec3::new(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    )
}

/// Compute the dot product of two vectors
pub fn dot<A: Index<usize, Output = f32>, B: Index<usize, Output = f32>>(a: &A, b: &B) -> f32 {
    a[0] * b[0] + a[1] * b[1] + a[2] * b[2]
}

/// Linear interpolation between `a` and `b` at some distance `t` where:
/// - `t` is in `[0, 1]`
/// - `t = 0` returns `a`
/// - `t = 1` returns `b`
pub fn lerp<T: Mul<f32, Output = T> + Add<Output = T> + Copy>(t: f32, a: &T, b: &T) -> T {
    *a * (1.0 - t) + *b * t
}

/// Clamp `x` to be between `min` and `max`
pub fn clamp<T: PartialOrd>(x: T, min: T, max: T) -> T {
    if x < min {
        min
    } else if x > max {
        max
    } else {
        x
    }
}
/// Compute the direction specified by `theta` and `phi` in the spherical coordinate system
pub fn spherical_dir(sin_theta: f32, cos_theta: f32, phi: f32) -> Vec3 {
    Vec3::new(
        sin_theta * f32::cos(phi),
        sin_theta * f32::sin(phi),
        cos_theta,
    )
}
/// Compute the direction specified by `theta` and `phi` in the coordinate system
/// formed by `x`, `y` and `z`
pub fn spherical_dir_coords(
    sin_theta: f32,
    cos_theta: f32,
    phi: f32,
    x: &Vec3,
    y: &Vec3,
    z: &Vec3,
) -> Vec3 {
    sin_theta * f32::cos(phi) * *x + sin_theta * f32::sin(phi) * *y + cos_theta * *z
}
/// Compute the value of theta for the vector in the spherical coordinate system
pub fn spherical_theta(v: &Vec3) -> f32 {
    f32::acos(clamp(v.z, -1f32, 1f32))
}
/// Compute the value of phi for the vector in the spherical coordinate system
pub fn spherical_phi(v: &Vec3) -> f32 {
    match f32::atan2(v.y, v.x) {
        x if x < 0f32 => x + f32::consts::PI * 2.0,
        x => x,
    }
}
/// Try to solve the quadratic equation `a*t^2 + b*t + c = 0` and return the two
/// real roots if a solution exists
pub fn solve_quadratic(a: f32, b: f32, c: f32) -> Option<(f32, f32)> {
    let discrim_sqr = b * b - 4.0 * a * c;
    if discrim_sqr < 0.0 {
        None
    } else {
        let discrim = f32::sqrt(discrim_sqr);
        let q = if b < 0.0 {
            -0.5 * (b - discrim)
        } else {
            -0.5 * (b + discrim)
        };
        match (q / a, c / q) {
            (x, y) if x > y => Some((y, x)),
            (x, y) => Some((x, y)),
        }
    }
}
/// Compute a local ortho-normal coordinate system from a single vector.
pub fn coordinate_system(e1: &Vec3) -> (Vec3, Vec3) {
    let e2 = if f32::abs(e1.x) > f32::abs(e1.y) {
        let inv_len = 1.0 / f32::sqrt(e1.x * e1.x + e1.z * e1.z);
        Vec3::new(-e1.z * inv_len, 0.0, e1.x * inv_len)
    } else {
        let inv_len = 1.0 / f32::sqrt(e1.y * e1.y + e1.z * e1.z);
        Vec3::new(0.0, e1.z * inv_len, -e1.y * inv_len)
    };
    let e3 = cross(e1, &e2);
    (e2, e3)
}
/// Compute the reflection of `w` about `v`, both vectors should be normalized
pub fn reflect(w: &Vec3, v: &Vec3) -> Vec3 {
    2.0 * dot(w, v) * *v - *w
}
/// Compute the refraction of `w` entering surface with normal `n` where
/// the refractive index in the incident material is `eta_1` and the refractive
/// index of the entered material is `eta_2`. In the case of total internal
/// refraction this will return None.
pub fn refract(w: &Vec3, n: &Vec3, eta_1: f32, eta_2: f32) -> Option<Vec3> {
    let eta = eta_1 / eta_2;
    let cos_t1 = dot(n, w);
    let sin_t1_sqr = f32::max(0.0, 1.0 - f32::powf(cos_t1, 2.0));
    let sin_t2_sqr = f32::powf(eta, 2.0) * sin_t1_sqr;
    if sin_t2_sqr >= 1.0 {
        None
    } else {
        let cos_t2 = f32::sqrt(1.0 - sin_t2_sqr);
        Some(eta * -*w + (eta * cos_t1 - cos_t2) * *n)
    }
}

#[cfg(test)]
mod tests {
    use linalg::*;

    #[test]
    fn test_cross_product() {
        let a = Vec3::new(1f32, 0f32, 0f32);
        let b = Vec3::new(0f32, 1f32, 0f32);
        assert_eq!(cross(&a, &b), Vec3::new(0f32, 0f32, 1f32));
    }

    #[test]
    fn test_dot_product() {
        let a = Vec3::new(1f32, 0f32, 0f32);
        let b = Vec3::new(0f32, 1f32, 0f32);
        assert_eq!(dot(&a, &b), 0f32);
    }

    #[test]
    fn test_lerp() {
        let a = Vec3::new(1f32, 0f32, 0f32);
        let b = Vec3::new(0f32, 1f32, 0f32);
        assert_eq!(lerp(0.5, &a, &b), Vec3::new(0.5f32, 0.5f32, 0f32));
    }

    #[test]
    fn test_clamp() {
        assert_eq!(clamp(1f32, 0f32, 2f32), 1f32);
        assert_eq!(clamp(-1f32, 0f32, 2f32), 0f32);
        assert_eq!(clamp(3f32, 0f32, 2f32), 2f32);
    }

    #[test]
    fn test_spherical_dir() {
        let calcutated = spherical_dir(1f32, 0f32, f32::consts::FRAC_PI_2);
        let reference = Vec3::new(0f32, 1f32, 0f32);
        assert_approx_eq!(calcutated[0], reference[0]);
        assert_approx_eq!(calcutated[1], reference[1]);
        assert_approx_eq!(calcutated[2], reference[2]);
    }
}