package com.Starygin.service.security;

import com.Starygin.model.entity.Pet;
import com.Starygin.model.repository.PetRepository;
import com.Starygin.service.exceptions.PetNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.access.PermissionEvaluator;
import org.springframework.security.core.Authentication;
import org.springframework.stereotype.Component;

import java.io.Serializable;
import java.util.Optional;

@Component
public class PetPermissionEvaluator implements PermissionEvaluator {

    private final PetRepository petRepository;

    @Autowired
    public PetPermissionEvaluator(PetRepository petRepository) {
        this.petRepository = petRepository;
    }

    @Override
    public boolean hasPermission(Authentication authentication, Object targetDomainObject, Object permission) {
        return false;
    }

    @Override
    public boolean hasPermission(Authentication authentication, Serializable targetId, String targetType, Object permission) {
        String p = (String) permission;

        boolean admin = authentication.getAuthorities().stream()
                .anyMatch(a -> a.getAuthority().equals(p));

        Long ownerId = ((UserDetailsImpl) authentication.getPrincipal()).GetOwnerId();

        return admin
                || petPermission(targetId, targetType, ownerId)
                || ownerPermission(targetId, targetType, ownerId);
    }

    private boolean petPermission(Serializable targetId, String targetType, Long ownerId) {
        if (!targetType.equals("pet")) {
            return false;
        }

        Long petId = (Long) targetId;

        Optional<Pet> pet = petRepository.findById(petId);
        if (pet.isEmpty()) {
            throw new PetNotFoundException();
        }

        return pet.get().getOwner().getId().equals(ownerId);
    }

    private boolean ownerPermission(Serializable targetId, String targetType, Long ownerId) {
        if (!targetType.equals("owner")) {
            return false;
        }

        Long target = (Long) targetId;

        return target.equals(ownerId);
    }
}
