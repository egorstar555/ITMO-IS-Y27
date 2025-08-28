package com.starygin.ownersms.service.dto;

import java.time.LocalDate;

public record OwnerDto(Long id, String name, LocalDate birthday, Long userId) {
}