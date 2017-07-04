//! Vector is a standard 3 component vector
//!
//! The file is shamelessly copied and adapted from
//! https://github.com/Twinklebear/tray_rust/blob/master/src/linalg/vector.rs
//! which is licensed under MIT
//!
//! # Examples
//!
//! ```
//! use cg3_path_tracer::linalg::vector::Vector;
//!
//! let a = Vector::new(1f32, 2f32, 3f32);
//! let b = Vector::new(1f32, 2f32, 3f32);
//!
//! println!("{:?}", a + b);
//! println!("{:?}", 2f32 * a);
//!
//! println!("{:?}", a.length()); // print the lenght of the vector
//! println!("{:?}", a.normalized()); // print the normalized version of the vector
//!
//! println!("{:?}", a[2]); // will print the z value of a
//! println!("{:?}", a.z); // will print the  z value of a
//! ```


use std::f32;

use std::ops::{Add, Sub, Mul, Div, Neg, Index, IndexMut};

/// Vector is a standard 3 component vector
#[derive(Debug, Copy, Clone, PartialEq, PartialOrd)]
pub struct Vector {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

impl Vector {
    /// Initialize the vector and set values for x, y, z
    pub fn new(x: f32, y: f32, z: f32) -> Vector {
        Vector { x: x, y: y, z: z }
    }
    /// Initialize the vector with the same value of x, y, z
    pub fn broadcast(x: f32) -> Vector {
        Vector { x: x, y: x, z: x }
    }
    /// Compute the squared length of the vector
    pub fn length_sqr(&self) -> f32 {
        self.x * self.x + self.y * self.y + self.z * self.z
    }
    /// Compute the length of the vector
    pub fn length(&self) -> f32 {
        f32::sqrt(self.length_sqr())
    }
    /// Get a normalized copy of this vector
    pub fn normalized(&self) -> Vector {
        let len = self.length();
        Vector {
            x: self.x / len,
            y: self.y / len,
            z: self.z / len,
        }
    }
}

impl Add for Vector {
    type Output = Vector;
    /// Add two vectors together
    fn add(self, rhs: Vector) -> Vector {
        Vector {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
            z: self.z + rhs.z,
        }
    }
}

impl Sub for Vector {
    type Output = Vector;
    /// Subtract two vectors
    fn sub(self, rhs: Vector) -> Vector {
        Vector {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
            z: self.z - rhs.z,
        }
    }
}

impl Mul for Vector {
    type Output = Vector;
    /// Multiply two vectors
    fn mul(self, rhs: Vector) -> Vector {
        Vector {
            x: self.x * rhs.x,
            y: self.y * rhs.y,
            z: self.z * rhs.z,
        }
    }
}

impl Mul<f32> for Vector {
    type Output = Vector;
    /// Scale the vector by some value
    fn mul(self, rhs: f32) -> Vector {
        Vector {
            x: self.x * rhs,
            y: self.y * rhs,
            z: self.z * rhs,
        }
    }
}

impl Mul<Vector> for f32 {
    type Output = Vector;
    /// Scale the vector by some value
    fn mul(self, rhs: Vector) -> Vector {
        Vector {
            x: self * rhs.x,
            y: self * rhs.y,
            z: self * rhs.z,
        }
    }
}

impl Div for Vector {
    type Output = Vector;
    /// Divide the vectors components by the right hand side's components
    fn div(self, rhs: Vector) -> Vector {
        Vector {
            x: self.x / rhs.x,
            y: self.y / rhs.y,
            z: self.z / rhs.z,
        }
    }
}

impl Div<f32> for Vector {
    type Output = Vector;
    /// Divide the vectors components by a scalar
    fn div(self, rhs: f32) -> Vector {
        Vector {
            x: self.x / rhs,
            y: self.y / rhs,
            z: self.z / rhs,
        }
    }
}

impl Neg for Vector {
    type Output = Vector;
    /// Negate the vector
    fn neg(self) -> Vector {
        Vector {
            x: -self.x,
            y: -self.y,
            z: -self.z,
        }
    }
}

impl Index<usize> for Vector {
    type Output = f32;
    /// Read the vector by index
    /// - 0 : x
    /// - 1 : y
    /// - 2 : z
    fn index(&self, i: usize) -> &f32 {
        match i {
            0 => &self.x,
            1 => &self.y,
            2 => &self.z,
            _ => panic!("Invalid index into vector"),
        }
    }
}

impl IndexMut<usize> for Vector {
    /// Write the vector by index
    /// - 0 : x
    /// - 1 : y
    /// - 2 : z
    fn index_mut(&mut self, i: usize) -> &mut f32 {
        match i {
            0 => &mut self.x,
            1 => &mut self.y,
            2 => &mut self.z,
            _ => panic!("Invalid index into vector"),
        }
    }
}

#[cfg(test)]
mod tests {
    use linalg::vector::Vector;

    #[test]
    fn test_new() {
        assert_eq!(
            Vector::new(1f32, 2f32, 3f32),
            Vector {
                x: 1f32,
                y: 2f32,
                z: 3f32,
            }
        );
    }

    #[test]
    fn test_broadcast() {
        assert_eq!(
            Vector::broadcast(2f32),
            Vector {
                x: 2f32,
                y: 2f32,
                z: 2f32,
            }
        );
    }

    #[test]
    fn test_length() {
        assert_eq!(Vector::new(4f32, 3f32, 0f32).length(), 5f32);
    }

    #[test]
    fn test_normalized() {
        assert_eq!(
            Vector::new(4f32, 3f32, 0f32).normalized(),
            Vector::new(4f32 / 5f32, 3f32 / 5f32, 0f32 / 5f32)
        );
    }

    #[test]
    fn test_add() {
        assert_eq!(
            Vector::new(1f32, 2f32, 3f32) + Vector::new(3f32, 2f32, 1f32),
            Vector::new(4f32, 4f32, 4f32)
        );
    }

    #[test]
    fn test_sub() {
        assert_eq!(
            Vector::new(1f32, 2f32, 3f32) - Vector::new(3f32, 2f32, 1f32),
            Vector::new(-2f32, 0f32, 2f32)
        );
    }

    #[test]
    fn test_neg() {
        assert_eq!(
            -Vector::new(3f32, 2f32, 1f32),
            Vector::new(-3f32, -2f32, -1f32)
        );
    }

    #[test]
    fn test_mul_elemwise() {
        assert_eq!(
            Vector::new(1f32, 2f32, 3f32) * Vector::new(3f32, 2f32, 1f32),
            Vector::new(3f32, 4f32, 3f32)
        );
    }

    #[test]
    fn test_mul_scal_left() {
        assert_eq!(
            2f32 * Vector::new(1f32, 2f32, 3f32),
            Vector::new(2f32, 4f32, 6f32)
        );
    }

    #[test]
    fn test_mul_scal_right() {
        assert_eq!(
            Vector::new(1f32, 2f32, 3f32) * 2f32,
            Vector::new(2f32, 4f32, 6f32)
        );
    }

    #[test]
    fn test_div_elemwise() {
        assert_eq!(
            Vector::new(4f32, 9f32, 16f32) / Vector::new(2f32, 3f32, 4f32),
            Vector::new(2f32, 3f32, 4f32)
        );
    }

    #[test]
    fn test_div_scal_right() {
        assert_eq!(
            Vector::new(4f32, 6f32, 8f32) / 2f32,
            Vector::new(2f32, 3f32, 4f32)
        );
    }

    #[test]
    fn test_idx_read_and_write() {
        let mut v = Vector::new(1f32, 2f32, 3f32);
        assert!(v[0] == 1f32 && v[1] == 2f32 && v[2] == 3f32);
        {
            let x = &mut v[1];
            *x = 5f32;
        }
        assert!(v[1] == 5f32);
    }
}