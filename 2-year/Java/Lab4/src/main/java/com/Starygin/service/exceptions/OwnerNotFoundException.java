package com.Starygin.service.exceptions;

public class OwnerNotFoundException extends RuntimeException {
    public OwnerNotFoundException() {
        super("Owner not found");
    }
}
