package com.starygin.petsms.service.exceptions;

public class PetIdMismatchException extends RuntimeException {

    public PetIdMismatchException() {
        super("Pet id mismatch");
    }

    public PetIdMismatchException(String message) {
        super(message);
    }
}
