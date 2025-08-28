package com.Starygin.service.exceptions;

public class PetNotFoundException extends RuntimeException {

    public PetNotFoundException() {
        super("Pet not found");
    }

    public PetNotFoundException(String message) {
        super(message);
    }
}
