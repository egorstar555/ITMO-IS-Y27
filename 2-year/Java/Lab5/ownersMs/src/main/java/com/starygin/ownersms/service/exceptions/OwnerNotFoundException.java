package com.starygin.ownersms.service.exceptions;

public class OwnerNotFoundException extends RuntimeException {
    public OwnerNotFoundException() {
        super("Owner not found");
    }
}
