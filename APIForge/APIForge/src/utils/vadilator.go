package main

import (
    "errors"
    "fmt"
    "regexp"
)

type Validator struct{}

func (v *Validator) ValidateEmail(email string) error {
    const emailRegex = `^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$`
    matched, err := regexp.MatchString(emailRegex, email)
    if err != nil {
        return fmt.Errorf("error matching email regex: %w", err)
    }
    if !matched {
        return errors.New("invalid email format")
    }
    return nil
}

func (v *Validator) ValidatePhone(phone string) error {
    const phoneRegex = `^\+?[1-9]\d{1,14}$`
    matched, err := regexp.MatchString(phoneRegex, phone)
    if err != nil {
        return fmt.Errorf("error matching phone regex: %w", err)
    }
    if !matched {
        return errors.New("invalid phone number format")
    }
    return nil
}

func (v *Validator) ValidateRequired(fieldName string, value string) error {
    if value == "" {
        return fmt.Errorf("%s is required", fieldName)
    }
    return nil
}

// Example usage
func main() {
    validator := Validator{}

    // Validate email
    email := "test@example.com" // Replace with actual email
    if err := validator.ValidateEmail(email); err != nil {
        fmt.Println("Email validation error:", err)
    } else {
        fmt.Println("Email is valid.")
    }

    // Validate phone
    phone := "+1234567890" // Replace with actual phone number
    if err := validator.ValidatePhone(phone); err != nil {
        fmt.Println("Phone validation error:", err)
    } else {
        fmt.Println("Phone number is valid.")
    }

    // Validate required field
    fieldName := "Username"
    value := "" // Replace with actual value
    if err := validator.ValidateRequired(fieldName, value); err != nil {
        fmt.Println("Validation error:", err)
    } else {
        fmt.Println("Validation passed.")
    }
}
